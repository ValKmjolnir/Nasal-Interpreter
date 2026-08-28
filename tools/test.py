import subprocess
import sys
from pathlib import Path
from dataclasses import dataclass

@dataclass
class TestResult:
    total: int
    passed: int

    def __add__(self, other: "TestResult") -> "TestResult":
        return TestResult(self.total + other.total, self.passed + other.passed)

def find_cpp_compiler() -> Path | None:
    find_list = ["clang++", "g++"]
    for compiler in find_list:
        if subprocess.call(["which", compiler], stdout=subprocess.DEVNULL) == 0:
            return Path(compiler)
    return None

def find_nasal_interpreter() -> Path | None:
    find_list = ["nasal", "nasal.exe"]
    for interpreter in find_list:
        if (Path(".") / interpreter).exists():
            return Path(".") / interpreter
        if (Path("build") / interpreter).exists():
            return Path("build") / interpreter
    return None

def test_cpp_tests(cpp_compiler: Path, files_dir: Path, include_directory: Path) -> TestResult:
    tr = TestResult(0, 0)
    test_executable = Path(".") / "test.exe"
    for file in files_dir.glob("*.cpp"):
        tr.total += 1
        print(f"[INFO] testing {file}")
        res = subprocess.run([str(cpp_compiler), "-std=c++17",
                              "-I", str(include_directory),
                              "-O3", "-o", str(test_executable), str(file)],
                             stdout=subprocess.DEVNULL,
                             stderr=subprocess.PIPE)
        if res.returncode != 0:
            print(f"[FAIL] {file} failed to compile:", res.stderr.decode())
            continue
        res = subprocess.run([str(test_executable)], stdout=subprocess.DEVNULL)
        if res.returncode == 0:
            tr.passed += 1
            print(f"[INFO] {file} passed")
        else:
            print(f"[INFO] {file} failed")
    if test_executable.exists():
        test_executable.unlink()
    return tr

def test_util_densemap(cpp_compiler: Path) -> TestResult:
    return test_cpp_tests(cpp_compiler, Path("test") / "util", Path("."))

def test_vm(cpp_compiler: Path) -> TestResult:
    return test_cpp_tests(cpp_compiler, Path("test") / "vm", Path("src"))

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
    print()
    print("=====================================")
    print(f"[INFO] {tr.passed}/{tr.total} passed")
    print("=====================================")

    if tr.total == tr.passed:
        sys.exit(0)
    else:
        sys.exit(1)
    