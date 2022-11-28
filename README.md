Flexiv App Guis
===============

This is a repository contains different UI applications and rdk programs that use flexiv_middleware2 as their communication infrastructure. 

Setup build environment with
-------------------------------
1. Install the build tools using ``sudo apt install build-essential pkg-config git wget autoconf libtool``

2. Clone submodules with ``git submodule update --init``

Configure and build the apps
-------------------------------

1. Install qt5 with ``sudo apt-get install qt5-default``
*(Optional)* If you need to use QT creator, you can install qt5 creator with ``sudo apt-get install qtcreator``

2. Clone and build ``flexiv_thirdparty2`` using its ``build_flexiv_sw_ubuntu_dependencies.sh`` script.

3. Configure the top-level CMake using ``cmake-gui``:  
````
        mkdir build && cd build
        cmake-gui .. &
````

4. Click Configure, then choose the default Unix Makefile configuration. An error will pop out, which is expected.

5. Configure ``flexiv_thirdparty2_path`` to the one cloned and built in step 2. For example, ``~/Documents/flexiv_thirdparty2``.

6. Choose the apps to build by checking options starting with ``BUILD_APP_``.

7. Click Configure then click Generate.

8. If no error occurs, close ``cmake-gui`` window, then go back to the terminal and:
````
        make -j4
````

9. All checked apps will be compiled and the executable binaries are under ``build/app/`` directory.
