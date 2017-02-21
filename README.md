# Twitter client for KDE Plasma 5
![plasmoid1](https://cloud.githubusercontent.com/assets/25787088/23183059/fd65f488-f87a-11e6-8e9a-e7297b43128f.png)

## Install
	1)
	Install libtwitcurl library
	See https://curl.haxx.se/libcurl/
	 OR
	For example on Ubuntu: "sudo apt-get install libcurl4-openssl-dev"

	2)
	Install liboauth library
	See https://github.com/x42/liboauth or https://sourceforge.net/projects/liboauth/
	 OR
	For example on Ubuntu: "sudo apt-get install liboauth-dev"

	3)
	To instal the client you can use "./build.sh" script. Or use CMake tool on your own. Without any option, eventual data in plasmoid location will be deleted, client will be compiled with release flags and installed to use in Plasmawidoved tool.  The script has following options.
	"-c" - Cleans plasmoid data and build data.
	"-r" - Installs plasmoid directly in plasma and restarts plasma to changes take effect.
	"-b" - Builds app only. App is not installed.
	"-v" - Compiles app with debug flags and runs it in plasmawindowed tool with valgrind tool.
	"-d" - Compiles with debug flags.
	"-t" - Prevents compiling Twitcurl lib. (Twitcurl lib is installed as a shared lib, after first run without this option.)

## Libs
	Client uses folowing libs(that are not my work!):
	- Twitcurl - https://github.com/swatkat/twitcurl - in folder: "./plugin/Twitter/libtwitcurl/*"
	- JSonCpp  - https://github.com/open-source-parsers/jsoncpp 

## Tested with configuration
	- Kubuntu 16.04 (64 bit)
	- KDE Plasma Version: 5.5.5 
	- Qt Version: 5.5.1
	- Compiler: g++ 5.4.0
	- Cmake version 3.5.1
	- Libcurl version: 7.47.0
	- Liboauth version: 1.0.3
