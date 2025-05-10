@echo off
REM ─────────────────────────────────────────
REM 1) vcpkg 경로 설정 (프로젝트 루트의 vcpkg 폴더)
vcpkg install gtest:x64-windows benchmark:x64-windows
vcpkg sdl2:x64-windows sdl2-ttf:x64-windows sdl2-image:x64-windows
vcpkg integrate install

REM 3) 솔루션 빌드
msbuild "%~dp0MyPuzzle.sln" /p:Configuration=Release /p:Platform=x64

REM 4) 단위 테스트 실행
echo.
echo === Running Unit Tests ===
"%~dp0x64\Release\Puzzle.Tests.exe" --gtest_output="json:%~dp0Tests.json"

REM 5) 벤치마크 실행
echo.
echo === Running Benchmarks ===
"%~dp0x64\Release\Puzzle.Benchmarks.exe" --benchmark_format=json --benchmark_out="%~dp0Benchmarks.json"

echo.
echo All done.
pause
