# ===================================================================
# PROJECT SETUP
# ===================================================================
project(kostal_qt_gui VERSION 0.1.0)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# ===================================================================
# PROJECT DEPENDENCIES
# ===================================================================
# empty for now

# ===================================================================
# MIDDLEWARE2 IDL GENERATION
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
# PROJECT EXECUTABLES
# ===================================================================
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src QT_SOURCE)
set(ALL_GUI_SOURCE ${QT_SOURCE} ${GENERATED_IDL_GUI_SOURCE} ${GENERATED_IDL_PLAN_SOURCE})
qt5_add_resources(ALL_GUI_SOURCE ${flexiv_app_guis_resources_path}/qt_flexiv_theme/resources.qrc)
add_executable(${PROJECT_NAME} ${ALL_GUI_SOURCE})
target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${GENERATED_IDL_GUI_INCLUDE_DIRECTORIES}
    ${GENERATED_IDL_PLAN_INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    flexiv::flexiv_app_guis
)
