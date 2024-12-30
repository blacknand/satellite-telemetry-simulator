## CMake build commands

- Set `CMAKE_PREFIX_PATH` to point to Qt installation

    ```bash
    cmake -B build -S . -DCMAKE_PREFIX_PATH=/Users/<your username>/Qt/6.8.1/macos
    ```

- Build

    ```bash
    cmake --build build
    ```

- Run 

    ```bash
    ./build/satellite_sim.app/Contents/MacOS/satellite_sim
    ```
## Using Qt and QtMaintenance
- If you get any CMake build errors such as ```Failed to find required Qt component "SerialPort". ``` then install it using the QtMaintenance app
