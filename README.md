# TimeM

Простое приложение Pomodoro + Task Manager с GUI на Dear ImGui.

## Сборка

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=Z:/CPP/TimeM/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```
