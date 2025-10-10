import os
import shutil

BUILD_DIR = "build"
if os.path.exists(f"{BUILD_DIR}\\nasal.exe"):
    shutil.move(f"{BUILD_DIR}\\nasal.exe", ".\\nasal.exe")
if os.path.exists(f"{BUILD_DIR}\\nasal-format.exe"):
    shutil.move(f"{BUILD_DIR}\\nasal-format.exe", ".\\nasal-format.exe")

for root, dirs, files in os.walk(f"{BUILD_DIR}"):
    for file in files:
        if file.endswith(".dll"):
            shutil.move(os.path.join(root, file), ".\\module\\" + file)