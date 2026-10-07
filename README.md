# BLAZAR

A procedural physical modelling synthesizer by Dog Lab Plugins for Windows x64 VST3 hosts. Forty factory programs combine four interacting worlds: MATTER resonators and exciters, harmonic FIELD structures, stochastic PARTICLES, and turbulent PLASMA. Coupling, evolution, tempo pulse and spatial diffusion shape the result. No sample library is required.

## Install on Windows

Close your DAW. Extract the release ZIP and copy the **entire Blazar.vst3 folder** to `C:\Program Files\Common Files\VST3\` (administrator permission may be needed). Keep its `Contents` directory intact. Restart your 64-bit VST3 host and rescan plugins; load **Blazar** as an instrument and send MIDI notes. No standalone application is included.

User patches are stored in `%APPDATA%\Dog Lab Plugins\Blazar\User Patches`. Factory programs are embedded. This release has its own plugin identity; legacy Quasar sessions are not migrated.

If your host reports missing Microsoft C++ runtime DLLs, install the current [Microsoft Visual C++ v14 x64 Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist). This candidate was built on Windows 10; other Windows/DAW combinations have not been certified.

## Build Windows x64 Release

Install Visual Studio Build Tools with Desktop development with C++, a Windows SDK, CMake 3.22 or newer, and Git. Obtain JUCE **8.0.12**, commit `29396c22c93392d6738e021b83196283d6e4d850`, separately and review its licence.

```powershell
git clone --branch 8.0.12 https://github.com/juce-framework/JUCE.git C:/dev/JUCE
./build.ps1 -JucePath C:/dev/JUCE
```

Run in a Visual Studio developer shell. The script configures an x64 build, builds the plugin and validation tools, then runs CTest. Output: `build-release/Blazar_artefacts/Release/VST3/Blazar.vst3`. Copy the whole bundle. JUCE is not downloaded automatically. The prepared plugin version is **1.0.0**; the proposed Git tag is **v1.0.0** (not created).

Pure DSP checks can be built without JUCE with `cmake -S . -B build-dsp -DQUASAR_DSP_ONLY=ON`, followed by a Release build and `ctest --test-dir build-dsp -C Release --output-on-failure`.

## Licensing

Publication is pending the owner's licensing and artwork decisions. See [LICENSE.md](LICENSE.md) and [PROVENANCE.md](PROVENANCE.md). No project-wide open-source licence has been selected. Third-party notices are in `third-party-notices/`.
