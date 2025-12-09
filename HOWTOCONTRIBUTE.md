## Prerequisites to Contribute

Before participating in the Ryan R-Type project, ensure you have the following installed on your system:

- **CMake**: Version 3.16 or higher
- **g++**: Version 9 or higher
- **Make**: Version 4.1 or higher
- **git**: Version 2.20 or higher
- **vcpkg**: Latest version

### Installation of Prerequisites

#### On Ubuntu/Debian-based systems:

```bash
sudo apt install cmake g++ make git build-essential cmake ninja-build libx11-dev libxi-dev libxrandr-dev libxcursor-dev libudev-dev libgl1-mesa-dev
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
export VCPKG_ROOT=$(pwd)    # add this to bashrc
```

#### On Fedora-based systems:

```bash
sudo dnf install cmake gcc-c++ make ninja-build libX11-devel libXi-devel libXrandr-devel libXcursor-devel systemd-devel mesa-libGL-devel
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
export VCPKG_ROOT=$(pwd)    # add this to bashrc
```

#### On macOS-based systems:

```bash
brew install cmake gcc make
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
export VCPKG_ROOT=$(pwd)
```

### Automated Installation with Script

Alternatively, after installing the system packages manually as described above, you can use the provided scripts to automate the installation of vcpkg:

#### On Unix-based systems (Linux/macOS):
```bash
./scripts/install_dependencies.sh
```

#### On Windows:
```batch
.\scripts\install_dependencies.bat
```

These scripts will clone and bootstrap vcpkg, and set the necessary environment variables.

## Commit Guidelines

This project follows the **Conventional Commit Message Guidelines**, which help maintain a clear and consistent Git history. The format is:

```
<type>(<scope>): <content>
```

### Common Types

- feat: A new feature
- fix: A bug fix
- docs: Documentation only changes
- style: Changes that do not affect the meaning of the code (white-space, formatting, etc.)
- refactor: Code changes that neither fix a bug nor add a feature
- test: Adding or correcting tests
- perf: A performance improvement
- upt: Updating an element that is not a refactor
- rm: Remove a line of code or a file from the repo
- memo: Add a new documentation element

## 🛠️ Git Commands Reference

### 🔄 Commit Management

**Modify commit message (before push):**
```bash
git commit --amend -m "New commit message"
```

**Modify commit message (after push):**
```bash
git commit --amend -m "New commit message"
git push --force
```

### 📂 File Management

**Unstage accidentally added file (not yet pushed):**
```bash
git restore --staged <file>
```

**Remove file from commit (after commit):**
```bash
git reset --soft HEAD~1
git restore --staged file-to-remove.txt
git commit -m "New commit message (without the file)"
```
