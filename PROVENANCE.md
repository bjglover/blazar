# Provenance

BLAZAR's DSP and factory programs are recorded as original procedural synthesis developed in this project. Modal mathematics, additive/phase synthesis, stochastic excitation and diffusion/delay methods were implemented locally. No sample library or third-party synthesizer implementation is recorded as incorporated.

Dependency: JUCE 8.0.12, commit `29396c22c93392d6738e021b83196283d6e4d850`, supplied separately. JUCE provides plugin, MIDI, GUI, state and audio infrastructure and includes VST3 SDK 3.8.0. See LICENSE.md for release terms and third-party-notices/ for upstream notices.

Artwork provenance and owner-confirmed generation provenance are recorded in Assets/ARTWORK.md. Historical checkpoints, generated user banks, renders, reports and development scripts are excluded from the public source set. Source/, the embedded artwork, CMake configuration and the selected validation tests suffice to rebuild this instrument; external JUCE and the Windows toolchain are also required.
