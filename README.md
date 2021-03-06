# DXVK

A Vulkan-based compatibility layer for Direct3D 11 which allows running 3D applications on Linux using Wine.

For the current status of the project, please refer to the [project wiki](https://github.com/doitsujin/dxvk/wiki).

For binary releases, see the [releases](https://github.com/doitsujin/dxvk/releases) page.

## Build instructions

### Requirements:
- [wine-staging](https://wine-staging.com/) for Vulkan support
- [Meson](http://mesonbuild.com/) build system
- [MinGW64](http://mingw-w64.org/) compiler and headers
- [glslang](https://github.com/KhronosGroup/glslang) front end and validator

### Building DLLs
Inside the dxvk directory, run:
```
# 64-bit build. For 32-bit builds, replace
# build-win64.txt with build-win32.txt
meson --cross-file build-win64.txt build.w64
cd build.w64
meson configure -Dprefix=/target/directory
# for an optimized release build:
meson configure -Dbuildtype=release
ninja
ninja install
```

Both `dxgi.dll` and `d3d11.dll`as well as some demo executables will be located in `/target/directory/bin`.

## How to use
In order to run `executable.exe` with DXVK,
* Copy `dxgi.dll` and `d3d11.dll` into the same directory as the executable
* Set `dxgi.dll` and `d3d11.dll` to *native* in your wine configuration. Note that *native,builtin* may not work for some applications.

In order to verify that your application uses DXVK instead of wined3d, check for the presence of the log files `d3d11.log` and `dxgi.log`, or enable the HUD.

### Online multi-player games
Manipulation of Direct3D libraries in multi-player games may be considered cheating and can get your account **banned**. This may also apply to single-player games with an embedded or dedicated multiplayer portion. **Use at your own risk.**

### Environment variables
The behaviour of DXVK can be modified with environment variables.

- `DXVK_DEBUG_LAYERS=1` Enables Vulkan debug layers. Highly recommended for troubleshooting and debugging purposes.
- `DXVK_SHADER_DUMP_PATH=directory` Writes all DXBC and SPIR-V shaders to the given directory
- `DXVK_LOG_LEVEL=error|warn|info|debug|trace` Controls message logging.
- `DXVK_HUD=1` Enables the HUD

## Samples and executables
In addition to the DLLs, the following standalone programs are included in the project:

- `d3d11-compute`: Runs a simple compute shader demo. Requires native `d3dcompiler_47.dll`.
- `d3d11-triangle`: Renders a bunch of triangles using D3D11. Requires native `d3dcompiler_47.dll`.
- `dxgi-factory`: Enumerates DXGI adapters and outputs for debugging purposes.
- `dxbc-compiler`: Compiles a DXBC shader to SPIR-V.
- `dxbc-disasm`: Disassembles a DXBC shader. Requires native `d3dcompiler_47.dll`.
- `hlsl-compiler`: Compiles a HLSL shader to DXBC. Requires native `d3dcompiler_47.dll`.
