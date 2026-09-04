import subprocess
import sys
import os
import difflib
from pathlib import Path
from dataclasses import dataclass

@dataclass
class TestResult:
    total: int
    passed: int

    def __add__(self, other: "TestResult") -> "TestResult":
        return TestResult(self.total + other.total, self.passed + other.passed)

def red(s: str) -> str:
    if not os.isatty(1):
        return s
    return "\033[91m" + s + "\033[0m"

def green(s: str) -> str:
    if not os.isatty(1):
        return s
    return "\033[92m" + s + "\033[0m"

def yellow(s: str) -> str:
    if not os.isatty(1):
        return s
    return "\033[93m" + s + "\033[0m"

def show_diff(expect: str, actual: str):
    diff = difflib.ndiff(
        expect.splitlines(),
        actual.splitlines()
    )

    print(yellow("==================== actual vs expected ===================="))
    for line in diff:
        if line.startswith("+"):
            print(green(line))
        elif line.startswith("-"):
            print(red(line))
        elif line.startswith("?"):
            print(yellow(line))
        else:
            print(line)
    print(yellow("========================= end diff ========================="))

def check(cmd: list[str], expect_stdout: str, expect_stderr: str) -> bool:
    res = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout = res.stdout.decode("utf-8")
    stderr = res.stderr.decode("utf-8")
    command = " ".join(cmd)
    if stdout != expect_stdout:
        print(f"{red('[ERROR-stdout]')}: {command}")
        show_diff(expect_stdout, stdout)
        return False
    if stderr != expect_stderr:
        print(f"{red('[ERROR-stderr]')}: {command}")
        show_diff(expect_stderr, stderr)
        return False
    return True

def find_cpp_compiler() -> Path | None:
    find_list = ["clang++", "g++"]
    for compiler in find_list:
        if subprocess.call(["which", compiler], stdout=subprocess.DEVNULL) == 0:
            return Path(compiler)
    return None

def find_nasal_interpreter() -> Path | None:
    find_list = ["nasal", "nasal.exe"]
    for interpreter in find_list:
        if (Path("build") / interpreter).exists():
            return Path("build") / interpreter
        if (Path(".") / interpreter).exists():
            return Path(".") / interpreter
    return None

def test_cpp_tests(cpp_compiler: Path, files_dir: Path, include_directory: Path) -> TestResult:
    tr = TestResult(0, 0)
    test_executable = os.path.join(".", "test.exe")
    for file in files_dir.glob("*.cpp"):
        tr.total += 1
        print(f"[INFO] testing {file}")
        res = subprocess.run([str(cpp_compiler), "-std=c++17",
                              "-I", str(include_directory),
                              "-O2", "-o", test_executable, str(file)],
                             stdout=subprocess.DEVNULL,
                             stderr=subprocess.PIPE)
        if res.returncode != 0:
            print(f"  [FAIL] {file} failed to compile:", res.stderr.decode())
            continue
        else:
            print(f"  [INFO] {file} successfully compiled")
        res = subprocess.run([test_executable], stdout=subprocess.DEVNULL)
        if res.returncode == 0:
            tr.passed += 1
            print(f"  [INFO] {file} passed")
        else:
            print(f"  [INFO] {file} failed")
    if os.path.exists(test_executable):
        os.remove(test_executable)
    return tr

def test_util_densemap(cpp_compiler: Path) -> TestResult:
    return test_cpp_tests(cpp_compiler, Path("test") / "util", Path("."))

def test_vm(cpp_compiler: Path) -> TestResult:
    return test_cpp_tests(cpp_compiler, Path("test") / "vm", Path("src"))

def test_nasal(nasal_interpreter: Path, files_dir: Path) -> TestResult:
    tr = TestResult(0, 0)
    for file in files_dir.glob("*.nas"):
        tr.total += 1
        print(f"[INFO] testing {file}")

        test_file_stdout = Path(str(file) + ".stdout")
        test_file_stderr = Path(str(file) + ".stderr")
        if not test_file_stdout.exists() and not test_file_stderr.exists():
            print(f"[FAILED] cannot find stdout/stderr file of {file}")
            continue

        cmd = [str(nasal_interpreter), str(file)]
        # expect stdout
        if test_file_stdout.exists():
            with open(test_file_stdout, "r") as f:
                stdout = f.read()
        else:
            stdout = ""
        # expect stderr
        if test_file_stderr.exists():
            with open(test_file_stderr, "r") as f:
                stderr = f.read()
        else:
            stderr = ""

        res = check(cmd, stdout, stderr)
        tr.passed += 1 if res else 0
        if res:
            print(green("  [PASSED]"), file)
        else:
            print(red("  [FAILED]"), file)
    return tr

if __name__ == "__main__":
    cpp_compiler = find_cpp_compiler()
    if cpp_compiler is None:
        print("cpp compiler not found")
        sys.exit(1)
    nasal_interpreter = find_nasal_interpreter()
    if nasal_interpreter is None:
        print("nasal interpreter not found")
        sys.exit(1)

    tr = test_util_densemap(cpp_compiler)
    tr += test_vm(cpp_compiler)
    tr += test_nasal(nasal_interpreter, Path("test") / "error")
    print()
    print("=====================================")
    print(f"[INFO] {tr.passed}/{tr.total} passed")
    print("=====================================")

    if tr.total == tr.passed:
        sys.exit(0)
    else:
        sys.exit(1)
    