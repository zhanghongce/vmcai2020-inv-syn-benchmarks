cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../configs/local.cfg)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/env.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/env.h)

# ---------------------------------------------------------------------------- #
# PROJECT
# name version language
# ---------------------------------------------------------------------------- #
project(PICO VERSION 1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------- #
# VARIABLE
# alias and configurations
# ---------------------------------------------------------------------------- #
set(PICO_TARGET ${PROJECT_NAME})

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
add_library(${PICO_TARGET}
  src/riscvIla.cc
  src/helpers.cc
  src/artifact_utility.cc
)

target_include_directories(${PICO_TARGET} PRIVATE include)

target_link_libraries(${PICO_TARGET} PUBLIC ilang::ilang)


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${PICO_TARGET}relchc
  app/main-relchc.cc
)


target_include_directories(${PICO_TARGET}relchc PRIVATE include)

target_link_libraries(${PICO_TARGET}relchc ${PICO_TARGET})

# ------------------------------------------------------------------- #

add_executable(${PICO_TARGET}pdrabc
  app/main-pdrabc.cc
)

target_include_directories(${PICO_TARGET}pdrabc PRIVATE include)

target_link_libraries(${PICO_TARGET}pdrabc ${PICO_TARGET})

# ------------------------------------------------------------------- #

add_executable(${PICO_TARGET}pdrchc
  app/main-pdrchc.cc
)

target_include_directories(${PICO_TARGET}pdrchc PRIVATE include)

target_link_libraries(${PICO_TARGET}pdrchc ${PICO_TARGET})

# ------------------------------------------------------------------- #

add_executable(${PICO_TARGET}cvc4sy
  app/main-cvc4sy.cc
)

target_include_directories(${PICO_TARGET}cvc4sy PRIVATE include)

target_link_libraries(${PICO_TARGET}cvc4sy ${PICO_TARGET})

# ------------------------------------------------------------------- #

add_executable(${PICO_TARGET}grain
  app/main-grain.cc
)

target_include_directories(${PICO_TARGET}grain PRIVATE include)

target_link_libraries(${PICO_TARGET}grain ${PICO_TARGET})