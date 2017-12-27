# Teensy 3.5 core with meson

This repo compile teensy 3.5 core (v1.35) with gcc-arm-7-2017-q4.

## Tools

* [Arm gcc (7-2017-q4)](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads)
* [Meson build](http://mesonbuild.com/)
* [Ninja build](https://ninja-build.org/)

## Compilation

```
meson build --cross-file teensy-3.5-cross.txt
cd build
ninja
```

## How to use

# PJRC
https://github.com/PaulStoffregen/cores.git

Teensy 2.0, 3.x, LC core libraries for Arduino.

The latest stable version of Teensy's core library is always available in the Teensyduino installer, at this page:

http://www.pjrc.com/teensy/td_download.html
