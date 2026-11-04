# raylibStart

Just a raylib project. Window opens, stuff renders.
с++23 - for some stuff

# structure
```
./
├── 3rdParty/raylib      # raylib (git submodule)
├── src/
│   ├── main.cpp         # entry point
│   ├── CMakeLists.txt   # build config
│   └── CMakePresets.json
├── build/               # build output (gitignored)
├── distribution/        # release exe goes here
├── .vscode/             # F5 debug config
├── build.bat            # build script
└── dist.bat             # build release + copy to distribution/
```

# clone

```bash
git clone --recurse-submodules <repo-url>
```

# build

```
build.bat          # Release (default)
build.bat -r       # Release (explicit)
build.bat -d       # Debug (with symbols, no optimization)
```

Exe lands in `build/Release/` or `build/Debug/`.

# distribution

```
dist.bat           # builds Release, copies exe to distribution/
```

# vscode

F5 saves all files, builds, and launches debugger.  
`launch.json` has Debug and Release configs.  
`tasks.json` has Build Debug/Release/Clean tasks.

- `ms-vscode.cpptools-extension-pack` — extension pack (C/C++ IntelliSense, debug, CMake presets, build, configure)

# requirements

- CMake
- Visual Studio 2022+ (MSVC)
