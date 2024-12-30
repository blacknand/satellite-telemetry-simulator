## CMake build commands
- Set `CMAKE_PREFIX_PATH` to point to Qt installation
    `cmake -B build -S . -DCMAKE_PREFIX_PATH=/Users/<your username>/Qt/6.8.1/macos`
- Build
    `cmake --build build`
- Run
    `cd satellite_sim.app/Contents/MacOS`