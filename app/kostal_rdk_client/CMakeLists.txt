# ===================================================================
#       PROJECT SETUP
# ===================================================================
project(kostal_rdk_client VERSION 0.1.0)

# ===================================================================
#       PROJECT DEPENDENCIES
# ===================================================================
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread system) 
find_package(spdlog REQUIRED)

# ===================================================================
#       MIDDLEWARE2 IDL GENERATION
# ===================================================================
# Generate middleware2 message source codes from idl files
flexiv_middleware2_fastrtpsgen(${CMAKE_CURRENT_SOURCE_DIR}/../../idl
    gui_msgs/msg
    ${CMAKE_CURRENT_BINARY_DIR}/generated
    GENERATED_IDL_GUI_HEADER
    GENERATED_IDL_GUI_SOURCE
    GENERATED_IDL_GUI_INCLUDE_DIRECTORIES)

flexiv_middleware2_fastrtpsgen(${CMAKE_CURRENT_SOURCE_DIR}/../../idl
    plan_msgs/msg
    ${CMAKE_CURRENT_BINARY_DIR}/generated
    GENERATED_IDL_PLAN_HEADER
    GENERATED_IDL_PLAN_SOURCE
    GENERATED_IDL_PLAN_INCLUDE_DIRECTORIES)

# ===================================================================
#       PROJECT EXECUTABLES
# ===================================================================
# add executables
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MAIN_SOURCE)
add_executable(${PROJECT_NAME} ${MAIN_SOURCE} ${GENERATED_IDL_GUI_SOURCE} ${GENERATED_IDL_PLAN_SOURCE})

# add path to all header files
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${GENERATED_IDL_GUI_INCLUDE_DIRECTORIES}
    ${GENERATED_IDL_PLAN_INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/flexiv_rdk/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/flexiv_rdk/thirdparty/eigen3
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# find and link dependencies
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    flexiv::flexiv_app_guis
    ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/flexiv_rdk/lib/libflexiv_rdk.x86_64-linux-gnu.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libjsoncpp.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libGinkgo_Driver.so
    Threads::Threads
    spdlog::spdlog
    ${Boost_LIBRARIES}
)

# ===================================================================
#       TEST EXECUTABLES
# ===================================================================
add_executable(test_client ${CMAKE_CURRENT_SOURCE_DIR}/test/test_client.cpp)
target_include_directories(test_client
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# find and link dependencies
target_link_libraries(test_client
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libjsoncpp.a
    ${Boost_LIBRARIES}
)