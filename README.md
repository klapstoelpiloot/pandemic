# Pandemic: The Arcade-Style Shuffle Board
This is the sourcecode for the [Pandemic shuffle board](http://www.codeimp.com/?p=pandemic). The code is written for this purpose only. The code is public for you to learn from or to be inspired. You may use it, but only at your own risk. There is no warranty. Feel free to use the discussions section in GitHub for questions.

The IOModule is made for the Arduino Nano Every. The Pandemic code is for the Raspberry Pi running Raspbian Lite (but it might work on other distros, I don't know). But the software is pretty useless without the hardware setup as described in the article. It needs the LED display and sensors and buttons via the Arduino, etc.

## Build & Deployment
The IOModule is easiest to compile and upload with the Arduino IDE, or with Visual Micro. It doesn't get more straight forward.

The Pandemic code is compiled on the Raspberry Pi using Visual Studio from a Windows machine connected to the same network. To make this work, you need to install the following packages on the Raspberry Pi:
> sudo apt install zip rsync openssh-server build-essential gdbserver git
Visual Studio automatically copies the changed files to the Raspberry Pi and remotely builds the project. The binaries are built in ~/projects/Pandemic/bin/ARM/ with a Debug or Release subdirectory depending on the chosen build configuration. There is a post-build action which should copy the required librgbmatrix.so library to the same output directory. This was previously built from Henner Zeller's [rpi-rgb-led-matrix library](http://www.github.com/hzeller/rpi-rgb-led-matrix/). Follow his instructions to build that library. Also the _pandemic.toml_ file is copied to the output directory as this must be read by the software on startup.

The data files must be in the directory /usr/local/games/data/, however, this is configurable in _pandemic.toml_.
