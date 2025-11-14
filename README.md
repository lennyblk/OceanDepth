# 🌊 Ocean Depth

An underwater terminal adventure game where you explore the ocean depths, fight marine creatures, and manage your oxygen to survive.

## 📦 Installation

### Option 1: Download Pre-compiled Binary (Recommended)

Go to the [**Releases**](https://github.com/lennyblackett/OceanDepth/releases) page and download the version for your system:

- **macOS (Apple Silicon M1/M2/M3/M4)**: `oceandepth_macos_arm64`
- **Windows**: `oceandepth_windows.exe`

#### On macOS:
```bash
chmod +x oceandepth_macos_arm64
./oceandepth_macos_arm64
```

#### On Windows:
Simply double-click on `oceandepth_windows.exe`

**No installation required!** Just download and run.

### Option 2: Compile from Source

If you want to compile the game yourself:

#### Prerequisites
- GCC or Clang
- Make

#### Steps
```bash
git clone https://github.com/lennyblackett/OceanDepth.git
cd OceanDepth
make clean && make
./OceanDepth
```

## 🎮 How to Play

- Explore the ocean depths
- Fight hostile creatures
- Manage your oxygen
- Upgrade your equipment
- Unlock special skills

## 🛠️ Development

### Project Structure
```
OceanDepth/
├── src/           # Source code
├── include/       # Header files
├── saves/         # Save files
└── Makefile       # Build script
```
