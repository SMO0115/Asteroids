import os
import sys
import subprocess
import shutil
import re
import platform
import multiprocessing

# --- CONFIGURATION ---
PROJECT_NAME = "Asteroids"
BUILD_DIR = "build"

# Detect CMake Generator
if shutil.which("ninja"):
    CMAKE_GENERATOR = "Ninja"
else:
    CMAKE_GENERATOR = "Visual Studio 17 2022" if platform.system() == "Windows" else "Unix Makefiles"

def run_command(command, cwd=None):
    try:
        subprocess.check_call(command, shell=True, cwd=cwd)
    except subprocess.CalledProcessError:
        print(f"[ERROR] Command failed: {command}")
        sys.exit(1)

def configure():
    if not os.path.exists(BUILD_DIR):
        os.makedirs(BUILD_DIR)
    print(f"[CONF] Configuring project with {CMAKE_GENERATOR}...")
    cmd = f'cmake -S . -B {BUILD_DIR} -G "{CMAKE_GENERATOR}"'
    run_command(cmd)

def build():
    if not os.path.exists(os.path.join(BUILD_DIR, "CMakeCache.txt")):
        configure()
    print("[BUILD] Compiling...")
    cpu_count = multiprocessing.cpu_count()
    cmd = f"cmake --build {BUILD_DIR} --config Debug --parallel {cpu_count}"
    run_command(cmd)

def run():
    build()
    print(f"[GAME] Launching {PROJECT_NAME}...")

    # Locate Executable
    exe_path = os.path.join(BUILD_DIR, "src", "game", PROJECT_NAME) # Linux nested
    if not os.path.exists(exe_path):
        exe_path = os.path.join(BUILD_DIR, "Debug", f"{PROJECT_NAME}.exe") # Windows

    if not os.path.exists(exe_path):
        # Fallback for flat builds
        exe_path = os.path.join(BUILD_DIR, PROJECT_NAME)

    if not os.path.exists(exe_path):
        print(f"[ERROR] Could not find executable.")
        sys.exit(1)

    if platform.system() != "Windows":
        os.chmod(exe_path, 0o755)

    run_command(f'"{exe_path}"', cwd=os.getcwd())

def format_code():
    """Runs clang-format on .cpp files ONLY."""
    print("[FMT] Formatting logic (*.cpp)...")

    if shutil.which("clang-format") is None:
        print("[ERROR] clang-format not found!")
        return

    # 1. Find ONLY .cpp files
    files = []
    for root, _, filenames in os.walk("src"):
        for filename in filenames:
            if filename.endswith(".cpp"):
                files.append(os.path.join(root, filename))

    if not files:
        print("[FMT] No .cpp files found.")
        return

    # 2. Run Clang-Format
    # -i edits files in place
    cmd = f"clang-format -i -style=file {' '.join(files)}"
    try:
        subprocess.check_call(cmd, shell=True, stdout=subprocess.DEVNULL)
        print(f"[FMT] Successfully formatted {len(files)} .cpp files.")
    except subprocess.CalledProcessError:
        print("[ERROR] Clang-format failed.")

def main():
    if len(sys.argv) < 2:
        print("Usage: python manage.py [run|build|format]")
        sys.exit(1)

    action = sys.argv[1]
    if action == "run": run()
    elif action == "build": build()
    elif action == "format": format_code()
    else: print("Unknown command")

if __name__ == "__main__":
    main()