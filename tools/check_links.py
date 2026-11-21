"""Check that every relative link and image in the repository's Markdown files
points to a file that exists. Exit code 1 if any link is broken.

Usage:  python tools/check_links.py
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SKIP_DIRS = {".git", ".metadata", "Blink_Project", "node_modules"}
LINK = re.compile(r"!?\[[^\]]*\]\(([^)\s]+)\)")


def markdown_files():
    for path in ROOT.rglob("*.md"):
        if not SKIP_DIRS.intersection(path.relative_to(ROOT).parts):
            yield path


def broken_links(md_file):
    text = md_file.read_text(encoding="utf-8")
    in_code = False
    for line in text.splitlines():
        if line.lstrip().startswith("```"):
            in_code = not in_code
            continue
        if in_code:
            continue
        for target in LINK.findall(line):
            if target.startswith(("http://", "https://", "mailto:", "#")):
                continue
            file_part = target.split("#", 1)[0]
            if file_part and not (md_file.parent / file_part).exists():
                yield target


def main():
    problems = 0
    count = 0
    for md_file in markdown_files():
        count += 1
        for target in broken_links(md_file):
            print(f"{md_file.relative_to(ROOT)}: broken link -> {target}")
            problems += 1
    print(f"Checked {count} Markdown files.")
    if problems:
        print(f"{problems} broken link(s).")
        return 1
    print("All links resolve.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
