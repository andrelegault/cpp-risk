# cpp-risk
C++ implementation of a strategy board game known as Warzone (version of Risk). As everything is randomized, game durations may vary a lot.

NOTE: Press `<enter>` if using observers to refresh the screen.

## Building Dependencies

* cmake >= 3.17.0
* make (mingw-make if on Windows)
* gcc >= 8

NOTE: Use `cmake -DCXX_COMPILER_VERSION=g++-8` to explicitly use a supported compiler version. This should be done if errors related to the `filesystem` module appear.

*Build files are located in the `./bin` directory.*

### Building (Linux)

Instructions:

```bash
cmake .
make
```

### Building (Windows)

You should download [cmake](https://cmake.org/download/) and [MinGW](https://osdn.net/projects/mingw/releases/). Makes sure to get the `base` and `g++` compiler. You'll want to add the MinGW binaries, which should be in `C:/MinGW/bin` to your `PATH`. Then you simply run:

Instructions:

```bash
cmake -G "MinGW Makefiles" .
mingw32-make
```

### Building (MacOS)

Install [Homebrew](https://brew.sh/) and run, `brew install cmake`

Instructions:

```bash
cmake .
make
```
