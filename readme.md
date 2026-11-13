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

<dl>
<dt><code>run_all.bat</code></dt>
<dd>sequantially <code>build</code> and <code>executes</code>:</dd>
<dd> - debug: <code>atGame</code>, <code>atTests</code>, <code>atTileTemplateGenerator</code></dd>
<dd> - release: <code>atGame</code>, <code>atTests</code></dd>
<dd> - dist: <code>atGame</code></dd>
<dt><code>gen_tileTemplates.bat</code></dt>
<dd>builds and runs <code>atTileTemplateGenerator</code> — generates tile grid PNGs into <code>./tileTemplates/</code></dd>
</dl>

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

<dl>
<dt><b>Source of truth</b></dt>
<dd> <code>.vscode/tasks.json</code> - how to build (commands right there)</dd>
<dd> <code>dist.bat</code> - how to build dist via .bat</dd>
<dd> <code>.vscode/launch.json</code> - where .exe lives</dd>
</dl>

<dl>
<dt><b>Cmake presets</b></dt>
<dd> <code>debug</code></dd>
<dd> <code>release</code></dd>
<dd> <code>dist</code> - same as release, except without console window</dd>
</dl>

<dl>
<dt><b>Targets</b></dt>
<dd> <code>atGame</code></dd>
<dd> <code>atTests</code></dd>
<dd> <code>atTileTemplateGenerator</code> - utility, generates tile grid templates</dd>
</dl>

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
