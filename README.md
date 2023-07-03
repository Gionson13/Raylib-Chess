# Chess

My version of chess made with [raylib](www.raylib.com).
This branch is a port of the master branch to android using [Raymob](https://github.com/Bigfoot71/Raymob).

## Prerequisites
The same as Raymob:
You will need SDK API 33.0.0 and NDK r21 (or higher, compatible with CMake 3.22.1).

When you have everything you can either use android studio or `gradlew` to build the project.

## Missing features
Comparing to the master branch there still are a few missing features:
- Unable to load a previous board (even tough you are able to save), for now it just crashes
- Settings menu during a game and vsync option (I dont understant how it works in android yet)

There are some features that are only for android that are missing:
- back button to exit game and application
- change icon to reflect the application
