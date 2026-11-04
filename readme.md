# AnimeTower

Just a raylib project. Window opens, stuff renders.
C++23 - for some stuff

# structure
```
./
├── 3rdParty/raylib      # raylib (git submodule)
├── src/
│   ├── main.cpp         # entry point
│   ├── CMakeLists.txt   # build config
│   └── CMakePresets.json
├── resource/            # fonts and other resources
├── build/               # build output (gitignored)
├── distribution/        # release exe goes here (gitignored)
├── .vscode/             # F5 debug config
├── build.bat            # build script
└── dist.bat             # build release (no console) + copy to distribution/
```

# clone

```bash
git clone --recurse-submodules <repo-url>
```

# build

```
build.bat          # Release (default, with console)
build.bat -r       # Release (explicit, with console)
build.bat -d       # Debug (with console, with symbols, no optimization)
build.bat -dist    # Release without console (for distribution)
```

Exe lands in `build/Release/` or `build/Debug/`.

# distribution

```
dist.bat           # builds Release (no console), copies exe + resource/ to distribution/AnimeTower/, creates ZIP
```

Creates `distribution/AnimeTower_YYYY-MM-DD_HH-MM.zip` with current date and time.

Archive contents:
```
AnimeTower_2026-11-04_19-15.zip
└── AnimeTower/
    ├── AnimeTower.exe
    └── resource/
        └── ...
```

# vscode

F5 saves all files, builds, and launches debugger.
`launch.json` has Debug, Release, and Dist (no console) configs.
`tasks.json` has Build Debug/Release/Dist/Clean tasks.

- `ms-vscode.cpptools-extension-pack` — extension pack (C/C++ IntelliSense, debug, CMake presets, build, configure)

# requirements

- CMake
- Visual Studio 2022+ (MSVC)

# license

MIT © KachanVladislav — see [LICENSE](LICENSE)

Uses [raylib](https://github.com/raysan5/raylib) (zlib/libpng license)
