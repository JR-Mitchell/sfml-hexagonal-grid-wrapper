# SFML Hexagonal Grid Wrapper

C++ code using SFML 2D Graphics library.
Provides tools for generating terrain with an orthographic view on a hexagonal grid.
Each point on the grid is associated with a hexagonal unit cell in the 2D plane.

## Prerequisites

- SFML
- cmake

## Building the demo with `cmake`

First, install the correct version of SFML for your system from [the SFML website](https://www.sfml-dev.org).
Unzip the zipped file into the `lib` folder.

If you are from the future, and the version of SFML you have downloaded is above `2.5.1`, modify the line

```cmake
set(SFML_DIR "lib/SFML-2.5.1/lib/cmake/SFML")
```

to replace `2.5.1` with the required version. Alternatively, if you want to work with SFML saved in a different directory (for example, if you wish to use it in multiple projects), just change this line such that it directs to the correct folder.

Then, build the code with [cmake](https://cmake.org).
In a Linux bash terminal, this is done as follows:

```bash
(uid):(dir)/sfml-hexagonal-grid-wrapper$ cd build
(uid):(dir)/sfml-hexagonal-grid-wrapper/build$ cmake ..
(uid):(dir)/sfml-hexagonal-grid-wrapper/build$ make
```

This will produce an executable file named `HexGrid` in the `build` directory.

## Using in a project

You could, if you wished to, just modify `main.cpp`/`main.h` in `src` and add further `.cpp`/`.h` files to your needs.
This may, however, become a little cluttered.

An alternative solution would be to move `src` to `lib`,

```bash
(uid):(dir)/sfml-hexagonal-grid-wrapper$ mv src lib/grid
(uid):(dir)/sfml-hexagonal-grid-wrapper$ mkdir src
```

before writing your own code in the `src` directory, and then adding the necessary lines to `CMakeLists.txt` to ensure the files in `lib/grid` are included.

I know that there are nicer ways of packaging up C++ code for others to use, but with this project I'm choosing to prioritise the code first, and the ease of distribution/use second.
