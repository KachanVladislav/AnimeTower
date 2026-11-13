# AnimeTower

AnimeTower game with raylib. 

![AnimeTower screenshot](screenshot.png)

# Get source
```bash
# --recurse-submodules for raylib
git clone --recurse-submodules <repo-url>
```
# Requirements
- CMake
- Visual Studio 2022+ (MSVC)
- for comfy vscode: `ms-vscode.cpptools-extension-pack`

# Distribution (easy way to build and run)
* run `dist.bat`
* it creates:
```
./
└── distribution/
    ├── atGame/
    |    ├── resource/
    |    └── atGame.exe                 # Desired .exe
    └── atGame_YYYY-MM-DD_HH-MM.zip     # .zip of atGame folder
```
* might return an error (known issue):
  * Microsoft Defender scans the freshly built `.exe` and blocks archiving
  * `atGame.exe` works fine, only `.zip` created without `atGame.exe`
  * to fix(`YOUR RISKS`, if you need): exclude project folder from Defender scanning (run PowerShell **as Admin**):
    ```powershell
    Add-MpPreference -ExclusionPath "D:\useful\!projects_new\AnimeTower"
    ```

# Miscellaneous scripts
- **`run_all.bat`** — sequantially `build` and `executes`:
  - debug: `atGame`, `atTests`, `atTileTemplateGenerator`
  - release: `atGame`, `atTests`
  - dist: `atGame`
- **`gen_tileTemplates.bat`** — builds and runs `atTileTemplateGenerator` — generates tile grid PNGs into `./tileTemplates/`

# Project structure
```
./
├── 3rdParty/raylib         # raylib (git submodule)
├── src/
│   ├── main.cpp                      # atGame main()
│   ├── main_Tests.cpp                # atTests main()
│   ├── main_TileTemplateGenerator.cpp # atTileTemplateGenerator main()
│   ├── CMakeLists.txt
│   └── CMakePresets.json
├── resource/               # resources
├── build/                  # build output (gitignored)
├── dist.bat                # see "Distribution" section
├── run_all.bat             # build & run all configs
├── gen_tileTemplates.bat   # build & run TileTemplateGenerator
├── distribution/           # release output (gitignored)
├── tileTemplates/          # generated tile grids (gitignored)
└── .vscode/                # comfy vscode tasks, settings, launch config
```

# Build

> [!NOTE]
> `CMakeLists.txt` in `src/`, so all scripts should address it

- **Source of truth:**
  - `.vscode/tasks.json` — how to build (commands right there)
  - `dist.bat` — how to build dist via .bat
  - `.vscode/launch.json` — where .exe lives

- **Cmake presets:**
  - `debug`
  - `release`
  - `dist` — same as release, except without console window

- **Targets:**
  - `atGame`
  - `atTests`
  - `atTileTemplateGenerator` — utility, generates tile grid templates

**Build example:**
```bash
cd src
cmake --preset debug
cmake --build --preset debug --target atGame
cd ..
```


# license

MIT © KachanVladislav — see [LICENSE](LICENSE)

Uses [raylib](https://github.com/raysan5/raylib) (zlib/libpng license)
