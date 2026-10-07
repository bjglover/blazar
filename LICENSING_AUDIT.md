# Release licensing audit — 2026-10-07

Result: the audited BLAZAR 1.0.0 source and Windows VST3 can be distributed under **AGPLv3**, retaining compatible dependency notices and providing exact Corresponding Source as described in SOURCE.md. Artwork provenance has been confirmed by the owner. No unresolved redistribution term was identified in the audited contents.

## Scope and findings

| Files/components | Finding |
| --- | --- |
| Source/Engine.h, Dynamics.h, Limits.h, MatterModels.h, WorldModels.h, Parameters.h | Recorded original procedural DSP, parameters and 40 factory programs. Standard C++ and local headers; no copied synthesizer implementation, samples or external preset packs identified. AGPL-3.0-only. |
| Source/Processor.h, Processor.cpp, BlazarEditor.h, UserPatches.h | Recorded original plugin, native JUCE/vector GUI and patch code. External inputs are JUCE and the embedded PNG. AGPL-3.0-only. |
| Tests/BlazarEditorCheck.cpp, BlazarPluginCheck.cpp, DynamicsBench.cpp, DynamicsCheck.cpp, FieldProbe.cpp, ReducedSmoke.cpp | Original validation code using project/JUCE APIs and standard C++; AGPL-3.0-only. Historical tests are excluded. |
| CMakeLists.txt, build.ps1, .gitignore, .gitmodules; project Markdown documentation and NOTICE | Original project build configuration, documentation and notices; AGPL-3.0-only for original material. |
| Assets/BlazarAccretion.png and Assets/ARTWORK.md | Project-specific generated artwork, authorized by owner, AGPLv3 to the extent copyright applies. Original generated mockup remains untracked, ignored, absent from history and excluded from all distributions. |
| LICENSE | Complete unmodified AGPLv3 text from SPDX's official licence data; verbatim redistribution permitted. |
| third-party-notices/* | Full upstream licence files verified against the actual JUCE checkout; individual source copyright notices remain in JUCE. Full GPLv3 text is also retained for optional GPL components in the upstream source. |

The file/include/build audit found no additional bundled images, font files, sound recordings, sample packs, wavetables or external DSP libraries in the project source. UI fonts use JUCE's default OS font; Windows font files are not redistributed. Reports, screenshots, checkpoints, generated banks, private paths and unrelated development files are excluded by the public allowlist.

## Actual JUCE and VST3 build

JUCE checkout is clean at **8.0.12 / 29396c22c93392d6738e021b83196283d6e4d850**. The 13 linked modules are juce_audio_basics, juce_audio_devices, juce_audio_formats, juce_audio_plugin_client, juce_audio_processors, juce_audio_processors_headless, juce_audio_utils, juce_core, juce_data_structures, juce_events, juce_graphics, juce_gui_basics and juce_gui_extra. Every module header explicitly declares AGPLv3/Commercial; this release selects AGPLv3. Commercial seat/revenue requirements do not apply to that option. A zero price alone would not establish licence compliance.

| Incorporated dependency | Terms and notice |
| --- | --- |
| VST3 SDK 3.8.0: root, base, pluginterfaces, public.sdk | MIT. All four licence notices retained with Steinberg copyright, permission and disclaimer. No VST2 SDK or VST logo bundled. |
| FLAC | BSD-style; FLAC-LICENSE.txt. |
| Ogg/Vorbis | BSD-style; OGG-VORBIS-LICENSE.txt and VORBIS-COPYING.txt. |
| IJG JPEG | IJG terms; JPEG-README.txt and required acknowledgement in NOTICE. |
| libpng | libpng permissive licence; PNG-LICENSE.txt. |
| zlib | zlib; ZLIB-LICENSE.txt. |
| HarfBuzz | Old MIT; HARFBUZZ-COPYING.txt and original individual source headers. |
| SheenBidi | Apache-2.0; SHEENBIDI-LICENSE.txt and original copyright header in SHEENBIDI-COPYRIGHT.txt. No upstream NOTICE file found. |
| Windows APIs / MSVC runtime | System/runtime dependencies; Windows font, SDK and runtime DLL files are not included in the ZIP. Runtime imports/install prerequisites are documented. |

All listed component terms are compatible with this AGPLv3 combination. Generated VST3 definitions disable other plugin formats and ARA; JUCE_ASIO defaults to zero. LV2/AU hosting, OpenGL, JavaScript, webview and external curl are not enabled. FLAC/Ogg/JPEG/PNG use JUCE's embedded implementations. Windows fonts use DirectWrite, not FreeType/Fontconfig.

JUCE's upstream source also retains optional components not compiled here: ASIO and AAX offer GPLv3 alternatives; AudioUnitSDK and Oboe are Apache-2.0; LV2/Serd/Sord/Sratom/Lilv are ISC; CHOC is ISC with QuickJS MIT; GLEW is BSD with Mesa/Khronos MIT portions; Box2D is zlib; pslextensions is public domain. Original component terms remain in their source directories. AGPL section 13 permits combination with GPLv3 where applicable. These optional components are not additional binary dependencies.

## Release obligations

Retain copyright, licence, no-warranty and compatible dependency notices and provide full AGPLv3 text (sections 4–5). Convey the combined covered work under AGPLv3 without noncommercial restrictions or prohibitions on modification/redistribution. Preserve/mark upstream changes; JUCE is unmodified. Section 5(d) requires applicable interactive legal notices and contains an exception for inherited interfaces without such notices; the reused framework has no inherited About/legal screen. NOTICE supplies the project's copyright, rights and disclaimer alongside the binary. No GUI changes were made for release.

Provide exact Corresponding Source under section 6, including embedded assets, JUCE/SDK source and build instructions. The tagged repository, pinned JUCE submodule and explicit source directions implement the network-distribution arrangement in section 6(d); automatic GitHub ZIPs alone omit submodules. The publisher remains responsible for continued source access. General-purpose tools/System Libraries are excluded. There is no installation lock preventing modified builds.

Section 13's remote-source offer applies if a modified covered program supports users interacting remotely through a network. This instrument exposes local VST3/MIDI/GUI operation, not a remote service. These findings do not decide separate licensing questions about any particular proprietary DAW.

The audit checks actual contents, declared terms and recorded provenance; it does not independently prove authorship of every original line. The owner supplied artwork provenance and authorized the AGPL release. Primary terms are LICENSE, JUCE 8.0.12 LICENSE.md/module headers and the retained upstream dependency notices.
