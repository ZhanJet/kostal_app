-lexiv App Guis
===============

This is a repository contains different UI applications and rdk programs that use flexiv_middleware2 as their communication infrastructure.

Setup build environment
-----------------------

1. Install the build tools using:

````
    sudo apt update
    sudo apt install build-essential pkg-config git wget autoconf libtool
````
7
2. Install some necessary dependencies:

````
    sudo apt install -y libjsoncpp-dev libspdlog-dev libusb-0.1-4
    // Deleted libboost-dev due to we use boost of flexiv_thirdparty2 to ///install
````

3. Install qt5 with:

````
    sudo apt-get install qt5-default
````

*(Optional)* If you need to use QT creator, you can install qt5 creator with:

````
    sudo apt-get install qtcreator
````

4. Clone [flexiv_thirdparty2](https://bitbucket.org/FlexivTeam/flexiv_thirdparty2.git) with:

````
    git clone https://bitbucket.org/FlexivTeam/flexiv_thirdparty2.git
````

5. Go to the cloned flexiv_thirdparty2 folder and then build necessary libraries individually:

````
    cd flexiv_thirdparty2

    # bash <build_script> <lib_name>
    bash build_flexiv_sw_ubuntu_dependencies.sh foonathan_memory_vendor
    bash build_flexiv_sw_ubuntu_dependencies.sh fast-cdr
    bash build_flexiv_sw_ubuntu_dependencies.sh fast-dds
    bash build_flexiv_sw_ubuntu_dependencies.sh fast-dds-gen
    bash build_flexiv_sw_ubuntu_dependencies.sh fast-rtps-gen
````

Configure and build the apps
-------------------------------

1. Go to the main folder and clone submodules with:

````
    git submodule update --init
````

2. Configure the top-level CMake using ``cmake-gui``:

````
    mkdir build && cd build
    cmake-gui .. &
````

3. Click Configure, then choose the default Unix Makefile configuration. An error will pop out, which is expected.

4. Configure ``flexiv_thirdparty2_path`` to the cloned ``flexiv_thirdparty2`` folder. For example, ``~/Documents/flexiv_thirdparty2``.

5. Choose the apps to build by checking options starting with ``BUILD_APP_``.

6. Click Configure then click Generate.

7. If no error occurs, close ``cmake-gui`` window, then go back to the terminal and:

````
    make -j4
````

8. All checked apps will be compiled and the executable binaries are under ``build/app/`` directory.
