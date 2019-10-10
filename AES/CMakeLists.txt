cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../configs/local.cfg)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/env.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/env.h)

# ---------------------------------------------------------------------------- #
# PROJECT
# name version language
# ---------------------------------------------------------------------------- #
project(AES VERSION 1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------- #
# VARIABLE
# alias and configurations
# ---------------------------------------------------------------------------- #
set(AES_TARGET ${PROJECT_NAME})

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# ---------------------------------------------------------------------------- #
# External dependencies
# ---------------------------------------------------------------------------- #
##
## ilang
##
find_package(ilang REQUIRED)

##
## Python (required if CMake version < 3.12)
##
if(${CMAKE_VERSION} VERSION_LESS 3.12)

  if(NOT TARGET Python::Python)
    add_library(Python::Python IMPORTED INTERFACE)

    set_property(TARGET Python::Python 
      PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${PYTHON_INCLUDE_DIRECTORIES}
    )

    set_property(TARGET Python::Python
      PROPERTY INTERFACE_LINK_LIBRARIES ${PYTHON_LIBRARIES}
    )
    
  endif()

endif()


# ---------------------------------------------------------------------------- #
# TARGET
# library
# ---------------------------------------------------------------------------- #
add_library(${AES_TARGET}
  src/aes_128.cc
  src/aes_child.cc
  src/aes_ila.cc
  src/aes_util.cc
  src/artifact_utility.cc
)

target_include_directories(${AES_TARGET} PRIVATE include)

target_link_libraries(${AES_TARGET} PUBLIC ilang::ilang)


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${AES_TARGET}relchc
  app/main-relchc.cc
)


target_include_directories(${AES_TARGET}relchc PRIVATE include)

target_link_libraries(${AES_TARGET}relchc ${AES_TARGET})

# ------------------------------------------------------------------- #

add_executable(${AES_TARGET}pdrabc
  app/main-pdrabc.cc
)

target_include_directories(${AES_TARGET}pdrabc PRIVATE include)

target_link_libraries(${AES_TARGET}pdrabc ${AES_TARGET})

# ------------------------------------------------------------------- #

add_executable(${AES_TARGET}pdrchc
  app/main-pdrchc.cc
)

target_include_directories(${AES_TARGET}pdrchc PRIVATE include)

target_link_libraries(${AES_TARGET}pdrchc ${AES_TARGET})

# ------------------------------------------------------------------- #

add_executable(${AES_TARGET}cvc4sy
  app/main-cvc4sy.cc
)

target_include_directories(${AES_TARGET}cvc4sy PRIVATE include)

target_link_libraries(${AES_TARGET}cvc4sy ${AES_TARGET})

# ------------------------------------------------------------------- #

add_executable(${AES_TARGET}grain
  app/main-grain.cc
)

target_include_directories(${AES_TARGET}grain PRIVATE include)

target_link_libraries(${AES_TARGET}grain ${AES_TARGET})