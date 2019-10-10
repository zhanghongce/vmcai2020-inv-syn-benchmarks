cmake_minimum_required(VERSION 3.8)
include(${CMAKE_CURRENT_SOURCE_DIR}/../configs/local.cfg)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/env.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/env.h)

# ---------------------------------------------------------------------------- #
# PROJECT
# name version language
# ---------------------------------------------------------------------------- #
project(SP VERSION 1.0 LANGUAGES CXX)

# ---------------------------------------------------------------------------- #
# VARIABLE
# alias and configurations
# ---------------------------------------------------------------------------- #
set(SP_TARGET ${PROJECT_NAME})

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
add_library(${SP_TARGET}
  src/sp_ila.cc
  src/artifact_utility.cc
)

target_include_directories(${SP_TARGET} PRIVATE include)

target_link_libraries(${SP_TARGET} PUBLIC ilang::ilang)


# ---------------------------------------------------------------------------- #
# TARGET
# executable
# ---------------------------------------------------------------------------- #
add_executable(${SP_TARGET}relchc
  app/main-relchc.cc
)


target_include_directories(${SP_TARGET}relchc PRIVATE include)

target_link_libraries(${SP_TARGET}relchc ${SP_TARGET})

# ------------------------------------------------------------------- #

add_executable(${SP_TARGET}pdrabc
  app/main-pdrabc.cc
)

target_include_directories(${SP_TARGET}pdrabc PRIVATE include)

target_link_libraries(${SP_TARGET}pdrabc ${SP_TARGET})

# ------------------------------------------------------------------- #

add_executable(${SP_TARGET}pdrchc
  app/main-pdrchc.cc
)

target_include_directories(${SP_TARGET}pdrchc PRIVATE include)

target_link_libraries(${SP_TARGET}pdrchc ${SP_TARGET})

# ------------------------------------------------------------------- #

add_executable(${SP_TARGET}cvc4sy
  app/main-cvc4sy.cc
)

target_include_directories(${SP_TARGET}cvc4sy PRIVATE include)

target_link_libraries(${SP_TARGET}cvc4sy ${SP_TARGET})

# ------------------------------------------------------------------- #

add_executable(${SP_TARGET}grain
  app/main-grain.cc
)

target_include_directories(${SP_TARGET}grain PRIVATE include)

target_link_libraries(${SP_TARGET}grain ${SP_TARGET})