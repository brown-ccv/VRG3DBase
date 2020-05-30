
# Includes
find_path(MINVR_INCLUDE_DIR 
    api/MinVR.h
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/include/VRG3D
    ${CMAKE_INSTALL_PREFIX}/include/vrg3d
    ${CMAKE_INSTALL_PREFIX}/include 
    $ENV{MinVR_ROOT}/include/VRG3D
    $ENV{MinVR_ROOT}/include/vrg3d
    $ENV{MinVR_ROOT}/include
    /usr/local/include/VRG3D
    /usr/local/include/vrg3d
    /usr/local/include
)

find_path(MinVRG3D_INCLUDE_DIR 
    VRG3DApp.h
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/include/MinVR/plugins/G3D/VRG3D/include
    ${MINVR_PROJECT_ROOT}/plugins/G3D/VRG3D/include
    /usr/local/include/MinVR/plugins/G3D/VRG3D/include
    
)

# libMinVR
find_library(MINVR_OPT_LIBRARIES 
  NAMES 
    MinVR
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib/VRG3D/Release
    ${CMAKE_INSTALL_PREFIX}/lib/VRG3D
    ${CMAKE_INSTALL_PREFIX}/lib/vrg3d/Release
    ${CMAKE_INSTALL_PREFIX}/lib/vrg3d
    ${CMAKE_INSTALL_PREFIX}/lib/Release
    ${CMAKE_INSTALL_PREFIX}/lib
    $ENV{VRG3D_ROOT}/lib/Release
    $ENV{VRG3D_ROOT}/lib
    /usr/local/lib
)

find_library(MINVR_DEBUG_LIBRARIES 
  NAMES 
    MinVRd
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib/VRG3D/Debug
    ${CMAKE_INSTALL_PREFIX}/lib/VRG3D
    ${CMAKE_INSTALL_PREFIX}/lib/vrg3d/Debug
    ${CMAKE_INSTALL_PREFIX}/lib/vrg3d
    ${CMAKE_INSTALL_PREFIX}/lib/Debug
    ${CMAKE_INSTALL_PREFIX}/lib
    $ENV{VRG3D_ROOT}/lib/Debug
    $ENV{VRG3D_ROOT}/lib
    /usr/local/lib
)


set(VRG3D_INCLUDE_DIRS
    ${VRG3D_INCLUDE_DIR}
    ${VRBASE_INCLUDE_DIR}
)

unset(VRG3D_LIBRARIES)
if (VRG3D_OPT_LIBRARIES)
  list(APPEND VRG3D_LIBRARIES optimized ${VRG3D_OPT_LIBRARIES})
endif()

if (VRG3D_DEBUG_LIBRARIES)
  list(APPEND VRG3D_LIBRARIES debug ${VRG3D_DEBUG_LIBRARIES})
endif()

if (VRBASE_OPT_LIBRARIES)
  list(APPEND VRG3D_LIBRARIES optimized ${VRBASE_OPT_LIBRARIES})
endif()

if (VRBASE_DEBUG_LIBRARIES)
  list(APPEND VRG3D_LIBRARIES debug ${VRBASE_DEBUG_LIBRARIES})
endif()



include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBNAME_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(
    VRG3D  
    DEFAULT_MSG
    VRG3D_INCLUDE_DIRS
    VRG3D_LIBRARIES
)

mark_as_advanced(
    VRG3D_OPT_LIBRARIES
    VRG3D_DEBUG_LIBRARIES
    VRBASE_OPT_LIBRARIES
    VRBASE_DEBUG_LIBRARIES
    VRG3D_LIBRARIES
    VRG3D_INCLUDE_DIR
    VRBASE_INCLUDE_DIR
    VRG3D_INCLUDE_DIRS
)
