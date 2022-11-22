# Search flexiv_thirdparty2 for dependencies first.
# Search flexiv_3rdparty for dependencies second.
macro(FvrUseFlexivThirdParty2)

# add path to Find*.cmake for flexiv_thirdparty2, and
# add path to Find*.cmake for flexiv_3rdparty
list(APPEND CMAKE_MODULE_PATH ${flexiv_thirdparty2_path}/cmake/Modules
                              ${CMAKE_SOURCE_DIR}/cmake/Modules)
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# Add flexiv_thirdparty2 install folder to CMAKE_PREFIX_PATH
# CMake will prioritize the flexiv_thirdparty2 over apt install packages,
# because CMAKE_PREFIX_PATH is searched before /usr/lib.
# For details, see Confluence page "Intro to CMake find_package()"
list(APPEND CMAKE_PREFIX_PATH ${flexiv_thirdparty2_path}/install/${CMAKE_SYSTEM_NAME}/Release)

endmacro()
