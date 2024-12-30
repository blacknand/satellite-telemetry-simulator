## CMake build commands

- Set `CMAKE_PREFIX_PATH` to point to Qt installation

    ```bash
    cmake -B build -S . -DCMAKE_PREFIX_PATH=/Users/<your username>/Qt/6.8.1/macos
    ```

- Build

    ```bash
    cmake --build build
    ```

- Run using

    ```bash
    cd satellite_sim.app/Contents/MacOS
    ```

    Alternatively

    ```bash
    open satellite_sim.app
    ```
