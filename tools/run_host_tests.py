"""Build and run the host unit tests for the safety core.

Needs a C compiler on the PATH (gcc or clang). Nothing else is installed.

    python tools/run_host_tests.py

Compiled as C99 with warnings treated as errors, so a new warning fails CI.
Exit code: 0 all checks passed, 1 a check failed, 2 no compiler or build error.
"""

import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SOURCE = ROOT / "safety" / "test" / "test_safety.c"


def main():
    compiler = shutil.which("gcc") or shutil.which("clang") or shutil.which("cc")
    if compiler is None:
        print("No C compiler found (gcc, clang or cc).")
        return 2
    with tempfile.TemporaryDirectory() as tmp:
        exe = Path(tmp) / "test_safety.exe"
        build = [compiler, "-std=c99", "-Wall", "-Wextra", "-Werror", "-pedantic",
                 "-I", str(ROOT / "safety" / "include"), str(SOURCE), "-o", str(exe)]
        built = subprocess.run(build)
        if built.returncode != 0:
            return 2
        return 1 if subprocess.run([str(exe)]).returncode != 0 else 0


if __name__ == "__main__":
    sys.exit(main())
