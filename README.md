# SysPulse

SysPulse is a lightweight Windows desktop system monitor built with C++20 and Dear ImGui. Development is incremental; each version is compiled and verified before the next feature is started.

## v0.1.0: ImGui Window

The current milestone creates a GLFW/OpenGL3 window and displays a basic `Hello, SysPulse!` ImGui overlay. System metrics are not connected yet.

## Prerequisites

- Windows 10 or 11
- Visual Studio 2022 or newer with C++20 support
- CMake 3.20 or newer
- Internet access for CMake FetchContent to download GLFW and Dear ImGui
- OpenGL 3.3-capable graphics drivers

## Build and Run

From a normal Windows Command Prompt with CMake and a C++ compiler available in `PATH`:

```bat
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
build\bin\syspulse.exe
```

The executable is generated at `build\bin\syspulse.exe`. The MinGW runtime must be available in `PATH`, or its runtime DLLs must be placed beside the executable.

Visual Studio is not required to run the built application. To build without Visual Studio, install CMake, Git, and MinGW-w64, then add their `bin` directories to `PATH`. CMake downloads GLFW and Dear ImGui during the first configuration.

## Project Layout

- `src/main.cpp` — GLFW window and Dear ImGui render loop
- `include/` — shared interfaces for later milestones
- `platforms/` — planned Windows system collectors
- `tests/` — planned unit tests

## Version Tracker

- **v0.1.0** — Project setup and blank Dear ImGui window
- **v0.2.0** — Windows CPU and memory collector
- **v0.3.0** — Background polling and live dashboard
- **v0.4.0** — Error handling, cleanup, and documentation
