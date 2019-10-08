cmake_minimum_required(VERSION 3.8)

# ---------------------------------------------------------------------------- #
# PROJECT
# name version language
# ---------------------------------------------------------------------------- #
project(RC VERSION 1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------- #
# VARIABLE
# alias and configurations
# ---------------------------------------------------------------------------- #
set(RC_TARGET ${PROJECT_NAME})

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
add_library(${RC_TARGET}
  src/rc_ila.cc
  src/artifact_utility.cc
)

target_include_directories(${RC_TARGET} PRIVATE include)

target_link_libraries(${RC_TARGET} PUBLIC ilang::ilang)


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${RC_TARGET}relchc
  app/main-relchc.cc
)


target_include_directories(${RC_TARGET}relchc PRIVATE include)

target_link_libraries(${RC_TARGET}relchc ${RC_TARGET})

# ------------------------------------------------------------------- #

add_executable(${RC_TARGET}pdrabc
  app/main-pdrabc.cc
)

target_include_directories(${RC_TARGET}pdrabc PRIVATE include)

target_link_libraries(${RC_TARGET}pdrabc ${RC_TARGET})

# ------------------------------------------------------------------- #

add_executable(${RC_TARGET}pdrchc
  app/main-pdrchc.cc
)

target_include_directories(${RC_TARGET}pdrchc PRIVATE include)

target_link_libraries(${RC_TARGET}pdrchc ${RC_TARGET})

# ------------------------------------------------------------------- #

add_executable(${RC_TARGET}cvc4sy
  app/main-cvc4sy.cc
)

target_include_directories(${RC_TARGET}cvc4sy PRIVATE include)

target_link_libraries(${RC_TARGET}cvc4sy ${RC_TARGET})

# ------------------------------------------------------------------- #

add_executable(${RC_TARGET}grain
  app/main-grain.cc
)

target_include_directories(${RC_TARGET}grain PRIVATE include)

target_link_libraries(${RC_TARGET}grain ${RC_TARGET})