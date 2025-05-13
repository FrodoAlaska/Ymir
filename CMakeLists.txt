cmake_minimum_required(VERSION 3.27)
project(ymir)

### Project Variables ###
############################################################
set(YMIR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(YMIR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(YMIR_INCLUDES 
  ${YMIR_INCLUDE_DIR} 
)
############################################################

### CMake Variables ###
############################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
############################################################

### Project Sources ###
############################################################
set(YMIR_SOURCES 
  ${YMIR_SRC_DIR}/main.cpp
  ${YMIR_SRC_DIR}/ymir_project.cpp
)
############################################################

### Final Build ###
############################################################
add_executable(${PROJECT_NAME} ${YMIR_SOURCES})
############################################################

### Linking ###
############################################################
target_include_directories(${PROJECT_NAME} PUBLIC BEFORE ${YMIR_INCLUDES})
############################################################

### Compiling Options ###
############################################################
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
############################################################

### Library Install ###
############################################################
install(TARGETS ${PROJECT_NAME} ymir 
        DESTINATION bin)
############################################################
