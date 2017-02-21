#!/bin/bash
PLASMOID_NAME="cz.cvut.fit.twitterPlasmoid"

# switch c - If used don't clean c++ build files, clean only plasmoid folder.
# switch r - If used restart plasma and run on desktop not in separated window.

function cleanBuild 
{
	rm -rv ./*
}

function cleanPlasmoid 
{
	sudo rm -rfv /usr/share/plasma/plasmoids/"$PLASMOID_NAME"
}

function cleanAll 
{
	cleanBuild
	cleanPlasmoid
}

CLEAN_BUILD_FLAG=false
RESTART_PLASMA_FLAG=false
BUILD_FLAG=false
VALGRIND_FLAG=false
NOT_INSTALL_TWITCURL=false
DEBUG_TYPE_FLAG="Release"


while getopts "crbvdt" opt; do
    case $opt in
    c) CLEAN_BUILD_FLAG=true ;; 
	r) RESTART_PLASMA_FLAG=true ;;
	b) BUILD_FLAG=true ;;
	v) VALGRIND_FLAG=true 
	   DEBUG_TYPE_FLAG="Debug" ;;
	d) DEBUG_TYPE_FLAG="Debug" ;;
	t) NOT_INSTALL_TWITCURL=true ;;
    \?) >&2 echo "Usage: $0 [-c clean build] [-r restart plasma] [-b build] [-v run with valgrind] [-d debug compile mode] [-t not install twitcurl library]"
		exit 1;;
    esac
done


mkdir -p build
cd ./build && \

if [ $CLEAN_BUILD_FLAG = true ]
	then
	cleanAll
else
	cleanPlasmoid
fi

if [ $NOT_INSTALL_TWITCURL = false ]
	then 
	cd ../plugin/Twitter/libtwitcurl && make install && cd ../../../build
fi

cmake .. \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DCMAKE_BUILD_TYPE=$DEBUG_TYPE_FLAG \
    -DLIB_INSTALL_DIR=lib \
    -DKDE_INSTALL_USE_QT_SYS_PATHS=ON && \
make && \
sudo make install && \

if [ $RESTART_PLASMA_FLAG = true ]
	then
	killall plasmashell; plasmashell > /dev/null 2>&1 &	
elif [ $BUILD_FLAG = true ]
	then
	:
elif [ $VALGRIND_FLAG = true ]
	then
	valgrind --tool=memcheck --leak-check=full plasmawindowed "$PLASMOID_NAME"
else
	plasmawindowed "$PLASMOID_NAME"
fi
