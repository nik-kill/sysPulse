# SysPulse

SysPulse is a lightweight Windows desktop system monitor built with C++20 and Dear ImGui. Development is incremental; each version is compiled and verified before the next feature is started.

## v0.3.0: Live ImGui Dashboard

The current milestone connects the Windows collector to the ImGui application through a background `std::jthread`. The dashboard displays live CPU and memory gauges, pagefile information, and rolling history plots for the last 120 samples.

Snapshots are passed to the UI through a mutex-protected bounded queue. The UI uses non-blocking reads so rendering never waits for the collector.

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

To run the v0.2.0 collector verification program:

```bat
build\bin\syspulse_collector.exe
```

It prints CPU and memory values once per second. Press `Ctrl+C` to stop. The MinGW runtime must be available in `PATH`, or its runtime DLLs must be placed beside the executables.

Visual Studio is not required to run the built application. To build without Visual Studio, install CMake, Git, and MinGW-w64, then add their `bin` directories to `PATH`. CMake downloads GLFW and Dear ImGui during the first configuration.

## Project Layout

- `src/main.cpp` — GLFW window and Dear ImGui render loop
- `ui/dashboard_ui.*` — live metrics, gauges, and history plots
- `src/collector_console.cpp` — v0.2.0 collector verification program
- `include/syspulse/` — shared metric and collector interfaces
- `include/syspulse/poller.hpp` and `src/poller.cpp` — background telemetry polling
- `platforms/windows_collector.*` — Win32 CPU and memory collection
- `tests/` — planned unit tests

## Version Tracker

- **v0.1.0** — Project setup and blank Dear ImGui window (complete)
- **v0.2.0** — Windows CPU and memory collector with console verification (complete)
- **v0.3.0** — Background polling and live dashboard (current)
- **v0.4.0** — Error handling, cleanup, and documentation


<img width="658" height="310" alt="image" src="https://github.com/user-attachments/assets/429ff926-b355-4030-b639-ec42e4a124fb" />
<img width="875" height="385" alt="image" src="https://github.com/user-attachments/assets/fa3c84cf-8911-4e4e-8ab4-e8e4e2ed7e3e" />
<img width="1269" height="735" alt="image" src="https://github.com/user-attachments/assets/e26e6ffe-f0c5-4374-8d5f-4bad11299847" />


