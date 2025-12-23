import os
import sys
import subprocess
import shutil
import platform
import multiprocessing # Add this at the top

# --- CONFIGURATION ---
# CHANGE THIS to match the name inside your CMakeLists.txt: add_executable(Asteroids ...)
PROJECT_NAME = "Asteroids"
BUILD_DIR = "build"
CMAKE_GENERATOR = "Visual Studio 17 2022" if platform.system() == "Windows" else "Unix Makefiles"

if shutil.which("ninja"):
    CMAKE_GENERATOR = "Ninja"
else:
    CMAKE_GENERATOR = "Unix Makefiles"

def run_command(command, cwd=None):
    """Executes a shell command and stops if it fails."""
    print(f"[EXEC] {command}")
    try:
        # shell=True is required to recognize commands like 'cmake' in the path
        subprocess.check_call(command, shell=True, cwd=cwd)
    except subprocess.CalledProcessError:
        print(f"[ERROR] Command failed: {command}")
        sys.exit(1)

def configure():
    """Runs CMake to generate build files."""
    if not os.path.exists(BUILD_DIR):
        os.makedirs(BUILD_DIR)

    print(f"[CONF] Configuring project for {platform.system()}...")
    # -S . means source is here, -B build means build output goes there
    cmd = f'cmake -S . -B {BUILD_DIR} -G "{CMAKE_GENERATOR}"'
    run_command(cmd)

def build():
    """Compiles the source code using all CPU cores."""
    if not os.path.exists(os.path.join(BUILD_DIR, "CMakeCache.txt")):
        print("[WARN] Build directory missing. Configuring first...")
        configure()

    print("[BUILD] Compiling...")

    # Get number of CPU cores to speed up build
    cpu_count = multiprocessing.cpu_count()

    # --parallel uses all cores (Like CLion does)
    cmd = f"cmake --build {BUILD_DIR} --config Debug --parallel {cpu_count}"
    run_command(cmd)

def run():
    """Builds and then Launches the game."""
    build() # Ensure it is built first

    print(f"[GAME] Launching {PROJECT_NAME}...")

    # Linux Path Strategy: Check the nested source folder first
    # This is where CMake puts binaries when using add_subdirectory()
    exe_path = os.path.join(BUILD_DIR, "src", "game", PROJECT_NAME)

    # Fallback for Windows or flat builds
    if not os.path.exists(exe_path):
        exe_path = os.path.join(BUILD_DIR, "Debug", f"{PROJECT_NAME}.exe") # Windows

    # Validation
    if not os.path.exists(exe_path):
        print(f"[ERROR] Executable not found at: {exe_path}")
        print(f"       Checked also: {os.path.join(BUILD_DIR, PROJECT_NAME)}")
        sys.exit(1)

    # Make executable (Linux/Mac) just in case
    if platform.system() != "Windows":
        os.chmod(exe_path, 0o755)

    # CRITICAL: cwd=os.getcwd() runs the game from Project Root
    # This effectively "mounts" the assets/ folder so the game can see it.
    run_command(f'"{exe_path}"', cwd=os.getcwd())


def format_code():
    """Runs clang-format on all source code."""
    print("[FMT] Formatting code...")

    # Check if tool exists
    if shutil.which("clang-format") is None:
        print("[ERROR] clang-format not found! Install LLVM.")
        print("        Linux: sudo apt install clang-format")
        print("        Windows: It comes with Visual Studio or install LLVM")
        return

    # Find files
    files = []
    for root, _, filenames in os.walk("src"):
        for filename in filenames:
            if filename.endswith((".cpp", ".h", ".hpp")):
                files.append(os.path.join(root, filename))

    if not files:
        print("[FMT] No files found.")
        return

    # -i means "edit in place" (save the file)
    cmd = f"clang-format -i -style=file {' '.join(files)}"
    run_command(cmd)
    print(f"[FMT] formatted {len(files)} files.")


def main():
    if len(sys.argv) < 2:
        print("Usage: python manage.py [run|build|configure]")
        sys.exit(1)

    action = sys.argv[1]

    if action == "run":
        run()
    elif action == "build":
        build()
    elif action == "configure":
        configure()
    elif action == "format":
        format_code()
    else:
        print(f"Unknown command: {action}")

if __name__ == "__main__":
    main()