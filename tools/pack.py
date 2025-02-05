import tarfile
import pathlib
import os
import platform
import shutil

build_directory = pathlib.Path("build")
if not os.path.exists(build_directory):
    print("pack binaries failed: build directory not found")
    exit(-1)

nasal_executable = pathlib.Path("nasal")
nasal_format_executable = pathlib.Path("nasal-format")
nasal_standard_library = pathlib.Path("std")
if not os.path.exists(nasal_executable):
    print("pack binaries failed: nasal executable not found")
    exit(-1)
if not os.path.exists(nasal_format_executable):
    print("pack binaries failed: nasal-format executable not found")
    exit(-1)
if not os.path.exists(nasal_standard_library):
    print("pack binaries failed: nasal standard library not found")
    exit(-1)

nasal_module_directory = pathlib.Path("module")
if not os.path.exists(nasal_module_directory):
    print("pack binaries failed: nasal module directory not found")
    exit(-1)

dynamic_library_suffix = ""
if platform.system()=="Windows":
    dynamic_library_suffix = ".dll"
elif platform.system()=="Linux":
    dynamic_library_suffix = ".so"
elif platform.system()=="Darwin":
    dynamic_library_suffix = ".dylib"
else:
    print("pack binaries failed: unsupported platform")

nasal_modules = []
for m in ["libfib", "libkey", "libmat", "libnasock"]:
    path = pathlib.Path("module").joinpath(m + dynamic_library_suffix)
    if not os.path.exists(path):
        print("pack binaries failed: nasal module `{}` not found".format(m))
        exit(-1)
    lib = pathlib.Path("module").joinpath(m + ".nas")
    if not os.path.exists(lib):
        print("pack binaries failed: nasal module lib `{}.nas` not found".format(m))
        exit(-1)
    nasal_modules.append(path)
    nasal_modules.append(lib)


tar_file_name = "nasal"
if platform.system()=="Windows":
    tar_file_name += "-windows-x86_64"
elif platform.system()=="Linux":
    tar_file_name += "-linux-x86_64"
elif platform.system()=="Darwin":
    tar_file_name += "-macos-aarch64"
else:
    print("pack binaries failed: unsupported platform")

# create package directory in build directory and copy files needed
package_directory = build_directory.joinpath(tar_file_name)
if not os.path.exists(package_directory):
    os.mkdir(package_directory)
    os.mkdir(package_directory.joinpath("module"))

print("pack nasal executable")
shutil.copy(nasal_executable, package_directory.joinpath(nasal_executable))
shutil.copy(nasal_format_executable, package_directory.joinpath(nasal_format_executable))

print("pack nasal standard library")
shutil.copytree(nasal_standard_library, package_directory.joinpath(nasal_standard_library))

for m in nasal_modules:
    print("pack nasal module:", m)
    shutil.copy(m, package_directory.joinpath(m))

file = tarfile.open(name=tar_file_name + ".tar", mode="w")
file.add(package_directory)

print("pack succeeded")
file.close()
