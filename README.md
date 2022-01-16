# Pandemic: The Arcade-Style Shuffle Board
This is the sourcecode for the [Pandemic shuffle board](http://www.codeimp.com/?p=pandemic). The code is written for this purpose only. The code is public for you to learn from or to be inspired. You may use it, but only at your own risk. There is no warranty. Feel free to use the discussions section in GitHub for questions.

The IOModule is made for the Arduino Nano Every. The Pandemic code is for the Raspberry Pi running Raspbian Lite (but it might work on other distros, I don't know). But the software is pretty useless without the hardware setup as described in the article. It needs the LED display and sensors and buttons via the Arduino, etc.

## Build & Deployment
The IOModule is easiest to compile and upload with the Arduino IDE, or with Visual Micro. It doesn't get more straight forward.

The Pandemic code is compiled on the Raspberry Pi using Visual Studio from a Windows machine connected to the same network. To make this work, you need to install the following packages on the Raspberry Pi:
> sudo apt install zip rsync openssh-server build-essential gdbserver git

Visual Studio automatically copies the changed files to the Raspberry Pi and remotely builds the project. The binaries are built in ~/projects/Pandemic/bin/ARM/ with a Debug or Release subdirectory depending on the chosen build configuration.

There is a post-build action which should copy the required librgbmatrix.so library to the same output directory. This was previously built from Henner Zeller's [rpi-rgb-led-matrix library](https://github.com/hzeller/rpi-rgb-led-matrix). Follow his instructions to build that library.

I don't remember how I installed fmod, but I do have a directory named 'fmodstudioapi20106linux' and you can probably get it from the offical [fmod website](https://www.fmod.com).

Also the _pandemic.toml_ file is copied to the output directory as this must be read by the software on startup.

The data files must be in the directory /usr/local/games/data/, however, this is configurable in _pandemic.toml_.

## Credits
Credit to whom credit is due. I'm using the following software libraries:
- [rpi-rgb-led-matrix library](https://github.com/hzeller/rpi-rgb-led-matrix) for the LED-matrix display control _(not included in this repository)_.
- [fmod](https://www.fmod.com) for sound & music playback and recording _(not included in this repository)_.
- [GLM library](https://github.com/g-truc/glm) for vector math.
- [tweeny](https://github.com/mobius3/tweeny) for smooth animations movements.
- [nanopb](https://github.com/nanopb/nanopb) for communication protocol between Raspberry Pi and Arduino.
- [cpptoml](https://github.com/skystrife/cpptoml) for the TOML configuration file format.
- [lodepng](https://github.com/lvandeve/lodepng) for PNG file writing.
- [cxxopts](https://github.com/jarro2783/cxxopts) for command line parameter parsing.

The following tools and resources were used:
- [AngelCode BMFont](http://www.angelcode.com/products/bmfont/) to generate the font atlas images.
- [Paint.NET](https://www.getpaint.net/) for the images.
- [Audacity](https://www.audacityteam.org/) for sound editing.
- [Shadertoy](https://www.shadertoy.com/) for experimenting with visual effects.
