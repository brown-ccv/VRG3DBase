project(VRG3DBase)

# Using 3.9 to get a modern version of FindOpenGL.cmake
cmake_minimum_required (VERSION 3.9) 

#include(AutoBuild)
#AutoBuild_init()

# Dependencies that are auto-downloaded, built, and installed for you will go here.  So, you 
# may use this even if not planning to install this particular project on your system.
#if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT OR "${CMAKE_INSTALL_PREFIX}" STREQUAL "")
#    set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
#endif()

list(APPEND CMAKE_MODULE_PATH  ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_INSTALL_PREFIX})
include(MessageMacros)


set(CMAKE_CXX_STANDARD 17)
    if(COMMAND cmake_policy)
      cmake_policy(SET CMP0003 NEW)
    endif(COMMAND cmake_policy)
add_definitions(-D_HAS_STD_BYTE=0)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

#------------------ Plugins ------------------ 

#macro(optional_adddir optionflag description dependsnote dir)
#  h2("Configuring ${description}. ${dependsnote}")
#  option(${optionflag} "If enabled, includes ${description} in the build.  ${dependsnote}")
#  if (${optionflag})
#    message(STATUS "ON: Will build and install ${description}.")
#    add_subdirectory(${dir})
#  else()
#    message(STATUS "OFF: NOT building ${description}.  Choose it with '-D ${optionflag}=ON'.")
#  endif()
#endmacro()


#--------------------- MinVR_G3D Option -----------------------------

#message(STATUS "ON: Will build and install MINVR.")
#optional_adddir(
#  WITH_MIVR_G3D
#  "Auto Download and Link MinVR_G3D"
#  ""
#  "MinVR_G3D"
#)

#---------------------- Find & Add Dependencies ----------------------




set(EXTERNAL_CMAKE_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)



add_subdirectory(VRG3DBase)



