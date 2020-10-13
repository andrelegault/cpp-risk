# cpp-risk
C++ implementation of a strategy board game known as risk

## Cmake Build

You can use the following to build:

```bash
cmake .
make
```

Build files are released as `WarZone_Basename` in the `drivers` directory, e.g., `drivers/WarZone_CardsDriver`.

## Cmake Windows

You should download [cmake](https://cmake.org/download/) and [MinGW](https://osdn.net/projects/mingw/releases/). Makes sure to get the `base` and `g++` compiler. You'll want to add the MinGW binaries, which should be in `C:/MinGW/bin` to your `PATH`. Then you simply run:

```bash
cmake -G "MinGW Makefiles" .
mingw32-make
```

## Cmake MacOS

Install [Homebrew](https://brew.sh/) and run, `brew install cmake`

Run this in the project root:

```bash
cmake .
make
```