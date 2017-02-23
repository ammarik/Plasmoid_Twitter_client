# Twitter client for KDE Plasma 5
This client is designed for desktop environment KDE Plasma 5 as a widget that is clipped to the computer desktop. Application directly on the desktop shows the latest notifications and posts from Twitter, allows to post statuses, send or display private messages.
![plasmoid1](https://cloud.githubusercontent.com/assets/25787088/23183059/fd65f488-f87a-11e6-8e9a-e7297b43128f.png)

## Get and install release

[Get the latest release](https://github.com/ammarik/Plasmoid_Twitter_client/releases/latest), tere is sample `.deb` package for simple instalation on Linux systems derived from Debian. Package was tested on Kubuntu 16.04 64-bit. Install `dpkg -i DEB_PACKAGE`.

## Compile and install from source code
### Dependencies
![zavislosti](https://cloud.githubusercontent.com/assets/25787088/23183802/97940a0c-f87d-11e6-9a81-3535de0e78bf.png)
	
### Compile and install

#### 1) Install libtwitcurl library
See https://curl.haxx.se/libcurl/

OR

For example on Ubuntu: `sudo apt-get install libcurl4-openssl-dev`

#### 2) Install liboauth library
See https://github.com/x42/liboauth or https://sourceforge.net/projects/liboauth/

OR

For example on Ubuntu: `sudo apt-get install liboauth-dev`

#### 3) Install the client
To instal the client you can use `./build.sh` script. Or use CMake tool on your own. Without any option, eventual data in plasmoid location will be deleted, client will be compiled with release flags and installed to use in Plasmawidoved tool.  The script has following options.

`-c` - Cleans plasmoid data and build data.

`-r` - Installs plasmoid directly in plasma and restarts plasma to changes take effect.

`-b` - Builds app only. App is not installed.

`-v` - Compiles app with debug flags and runs it in plasmawindowed tool with valgrind tool.

`-d` - Compiles with debug flags.

`-t` - Prevents compiling Twitcurl lib. (Twitcurl lib is installed as a shared lib, after first run without this option.)

## Getting started
There are two options:
### 1) You have installed the deb 
- Add plasmoid to desktop.
- Click on the window of the client right mouse button and choose the settings.
- Click on "Generate PIN" and get pin using a web browser.
- Insert PIN, click on "Login" and thats it.
- Close the settings window.
- (Optionally you can change API credentials... see option 2.)

### 2) You have compiled the client from source code
- First you have to register the client at https://apps.twitter.com/. 
- Click on the window of the client right mouse button and choose the settings.
- Insert Consumer Key(API Key) and Consumer secret(API Secret). Click "Set API credentials"
- Click on "Generate PIN" and get pin using a web browser.
- Insert PIN, click on "Login" and thats it.
- Close the settings window.

## Libs
Client uses folowing libs(that are not my work!):
- Twitcurl - https://github.com/swatkat/twitcurl - in folder: `./plugin/Twitter/libtwitcurl/*`
- JSonCpp  - https://github.com/open-source-parsers/jsoncpp 

## Tested with configuration
	- Kubuntu 16.04 (64 bit)
	- KDE Plasma Version: 5.5.5 
	- Qt Version: 5.5.1
	- Compiler: g++ 5.4.0
	- Cmake version 3.5.1
	- Libcurl version: 7.47.0
	- Liboauth version: 1.0.3
