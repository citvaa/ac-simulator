# AC Simulator (OpenGL, GLFW, GLEW, FreeType)

2D air-conditioner remote simulator: vent animation, desired/current temperature displays, status icons (heating/cooling/target), condensate bowl you must empty, FPS overlay, and a custom remote-shaped cursor.

## Requirements
- Windows with GPU supporting OpenGL 3.3+
- Visual Studio 2022 (C++17)
- NuGet: `glfw` 3.4.0 and `glew-2.2.0` (restored via `packages.config`)
- FreeType 2.x (runtime `freetype.dll` already in `x64/Debug`; for rebuild supply FreeType include/lib)
- `stb_image` included at `Header/stb_image.h`

## Build
1) Open `ac-simulator.sln` in Visual Studio 2022.
2) Restore NuGet packages (GLFW, GLEW) if VS doesn't do it automatically (`nuget restore ac-simulator.sln` also works).
3) Ensure FreeType include/lib paths are set; place `freetype.dll` next to the produced exe (or in PATH).
4) Choose x64 Debug/Release and Build/Run.

## Run
After building, start from the repo root so shader relative paths resolve, e.g.:

```powershell
.\x64\Debug\ac-simulator.exe
```

## Controls
- Click the lamp (red circle) to power on/off.
- Click the arrow button: top half increases, bottom half decreases desired temperature.
- Arrow Up/Down keys do the same.
- Space empties the bowl and unlocks the unit when full.
- ESC closes the app.

## Simulation Logic
- Vent animates only when ON and the bowl isn’t full.
- Bowl fills over time while running; at max it shuts off and locks until you press Space.
- Displays: left = desired temp, middle = current temp, right = flame (heating), snowflake (cooling), or check (target reached).
- Bottom-right nameplate is rendered as a texture.

## Project Structure
- `Source/` core logic (`Main.cpp`, `State.cpp`, `TemperatureUI.cpp`, `Renderer2D.cpp`, `TextRenderer.cpp`, `Controls.cpp`, `Util.cpp`)
- `Header/` declarations and helpers (`stb_image.h`, `ft2build.h` include)
- `Shaders/` OpenGL shaders for base draw, text, and overlay
- `x64/<Config>/` build outputs once you compile (Debug/Release)

## Notes
- Default font: `C:\Windows\Fonts\arial.ttf` (see `TextRenderer.cpp`); change the path if unavailable.
