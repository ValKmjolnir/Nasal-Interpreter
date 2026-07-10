import os
import shutil

BUILD_DIR = "build"
if os.path.exists(f"{BUILD_DIR}\\Release\\nasal.exe"):
    shutil.move(f"{BUILD_DIR}\\Release\\nasal.exe", ".\\nasal.exe")
if os.path.exists(f"{BUILD_DIR}\\Release\\nasal-format.exe"):
    shutil.move(f"{BUILD_DIR}\\Release\\nasal-format.exe", ".\\nasal-format.exe")

for root, dirs, files in os.walk(f"{BUILD_DIR}\\Release"):
    for file in files:
        if file.endswith(".dll"):
            # move and rename `<name>.dll` to `lib<name>.dll`
            shutil.move(os.path.join(root, file), ".\\module\\lib" + file)