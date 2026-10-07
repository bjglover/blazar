param([Parameter(Mandatory=$true)][string]$JucePath)
$ErrorActionPreference = 'Stop'
Set-Location -LiteralPath $PSScriptRoot
cmake -S . -B build-release -A x64 "-DJUCE_LOCAL_SOURCE=$JucePath"
if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed' }
cmake --build build-release --config Release --target Blazar_VST3 PluginCheck EditorCheck DynamicsCheck ReducedSmoke --parallel 2
if ($LASTEXITCODE -ne 0) { throw 'Release build failed' }
ctest --test-dir build-release -C Release --output-on-failure
if ($LASTEXITCODE -ne 0) { throw 'Release validation failed' }
