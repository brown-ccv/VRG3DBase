project(VRG3DBase)

#include(AutoBuildG3D)
#AutoBuild_use_package_MinVRG3D(${PROJECT_NAME} PUBLIC)

#find_package(MinVR REQUIRED) # MinVR_INCLUDE_DIR, MinVR_LIBRARIES
#find_package(G3D REQUIRED) # G3D_INCLUDE_DIR, G3D_LIBRARIES


# Includes
find_path(MinVR_INCLUDE_DIR 
    api/MinVR.h
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/include/MinVR
	${CMAKE_INSTALL_PREFIX}/include
    ${MINVR_PROJECT_ROOT}/include
    /usr/local/include/MinVR/
    
)



find_path(MinVRG3D_INCLUDE_DIR 
    VRG3DApp.h
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/include/MinVR/plugins/G3D/VRG3D/include
    ${MINVR_PROJECT_ROOT}/plugins/G3D/VRG3D/include
    /usr/local/include/MinVR/plugins/G3D/VRG3D/include
    
)

# lib MinVR
find_library(MinVR_LIBRARY
  NAMES 
    MinVR
	MinVRd
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib/
	${CMAKE_INSTALL_PREFIX}/lib/MinVR/Release
	${CMAKE_INSTALL_PREFIX}/lib/MinVR/Debug
	${CMAKE_INSTALL_PREFIX}/lib/MinVR
    ${MINVR_PROJECT_ROOT}/lib/
    /usr/local/lib
	/usr/local/lib/MinVR
)

find_library(MinVRG3D_LIBRARY
  NAMES 
    MinVR_G3Dd
	MinVR_G3D
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib/
	${CMAKE_INSTALL_PREFIX}/lib/MinVR/Release
	${CMAKE_INSTALL_PREFIX}/lib/MinVR/Debug
    ${MINVR_PROJECT_ROOT}/lib/
    /usr/local/lib
	/usr/local/lib/MinVR
)


# Includes
find_path(G3D_INCLUDE_DIR 
    G3D/G3D.h
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/include/G3D-8.0
    ${CMAKE_INSTALL_PREFIX}/include/G3D
    ${CMAKE_INSTALL_PREFIX}/include 
    $ENV{G3D_ROOT}/include/G3D-8.0
    $ENV{G3D_ROOT}/include/G3D
    $ENV{G3D_ROOT}/include
    /usr/local/include/G3D-8.0
    /usr/local/include/G3D
    /usr/local/include
)


# libGLG3D
find_library(GLG3D_OPT_LIBRARIES 
  NAMES 
    GLG3D
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib/G3D-8.0/Release
    ${CMAKE_INSTALL_PREFIX}/lib/G3D-8.0
    ${CMAKE_INSTALL_PREFIX}/lib/G3D/Release
    ${CMAKE_INSTALL_PREFIX}/lib/G3D
    ${CMAKE_INSTALL_PREFIX}/lib/Release
    ${CMAKE_INSTALL_PREFIX}/lib
    $ENV{G3D_ROOT}/lib/Release
    $ENV{G3D_ROOT}/lib
    /usr/local/lib
)

find_library(GLG3D_DEBUG_LIBRARIES 
  NAMES 
    GLG3Dd
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib/G3D-8.0/Debug
    ${CMAKE_INSTALL_PREFIX}/lib/G3D-8.0
    ${CMAKE_INSTALL_PREFIX}/lib/G3D/Debug
    ${CMAKE_INSTALL_PREFIX}/lib/G3D
    ${CMAKE_INSTALL_PREFIX}/lib/Debug
    ${CMAKE_INSTALL_PREFIX}/lib
    $ENV{G3D_ROOT}/lib/Debug
    $ENV{G3D_ROOT}/lib
    /usr/local/lib
)   

# Includes
find_path(GLG3D_INCLUDE_DIR 
    GLG3D/GLG3D.h
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/include/G3D-8.0
    ${CMAKE_INSTALL_PREFIX}/include/G3D
    ${CMAKE_INSTALL_PREFIX}/include 
    $ENV{G3D_ROOT}/include/G3D-8.0
    $ENV{G3D_ROOT}/include/G3D
    $ENV{G3D_ROOT}/include
    /usr/local/include/G3D-8.0
    /usr/local/include/G3D
    /usr/local/include
)


# libG3D
find_library(G3D_OPT_LIBRARIES 
  NAMES 
    G3D
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib/G3D-8.0/Release
    ${CMAKE_INSTALL_PREFIX}/lib/G3D-8.0
    ${CMAKE_INSTALL_PREFIX}/lib/G3D/Release
    ${CMAKE_INSTALL_PREFIX}/lib/G3D
    ${CMAKE_INSTALL_PREFIX}/lib/Release
    ${CMAKE_INSTALL_PREFIX}/lib
    $ENV{G3D_ROOT}/lib/Release
    $ENV{G3D_ROOT}/lib
    /usr/local/lib
)

find_library(G3D_DEBUG_LIBRARIES 
  NAMES 
    G3Dd
  HINTS 
    ${CMAKE_INSTALL_PREFIX}/lib/G3D-8.0/Debug
    ${CMAKE_INSTALL_PREFIX}/lib/G3D-8.0
    ${CMAKE_INSTALL_PREFIX}/lib/G3D/Debug
    ${CMAKE_INSTALL_PREFIX}/lib/G3D
    ${CMAKE_INSTALL_PREFIX}/lib/Debug
    ${CMAKE_INSTALL_PREFIX}/lib
    $ENV{G3D_ROOT}/lib/Debug
    $ENV{G3D_ROOT}/lib
    /usr/local/lib
)       

set(MINVR_VRGG3D_INCLUDE_DIRS ${MinVR_INCLUDE_DIR} ${MinVRG3D_INCLUDE_DIR} )
set(MINVR_VRGG3D_LIBRARIES ${MinVR_LIBRARIES} ${MinVRG3D_LIBRARY})


mark_as_advanced(
    MinVR_LIBRARY
	MinVR_INCLUDE_DIR
)

#---------------------- Find & Add Dependencies ----------------------
message(STATUS "Init VRG3DBase build")

set(EXTERNAL_CMAKE_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_library(
  VRG3DBase
  STATIC
  src/ConfigVal.cpp
  src/CovarianceMatrix.cpp
  src/EventMgr.cpp
  src/Fsa.cpp
  src/FsaHelper.cpp
  src/GfxMgr.cpp
  src/LoadingScreen.cpp
  src/Shadows.cpp
  src/SMesh.cpp
  src/StringUtils.cpp
  src/TexPerFrameSMesh.cpp
  src/TextFileReader.cpp
  src/ViewerHCI.cpp
  src/VRG3DBaseApp.cpp
  include/ConfigVal.H
  include/CovarianceMatrix.H
  include/EventFilter.H
  include/EventMgr.H
  include/Fsa.H
  include/FsaHelper.H
  include/GfxMgr.H
  include/GfxMgrCallbacks.H
  include/LoadingScreen.H
  include/Shadows.H
  include/SMesh.H
  include/StringUtils.H
  include/TexPerFrameSMesh.H
  include/TextFileReader.H
  include/ViewerHCI.H
  include/VRG3DBaseApp.h
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  # Windows specific code
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_HAS_AUTO_PTR_ETC=1)
  add_definitions(-DUNICODE -D_UNICODE)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set_target_properties(VRG3DBase PROPERTIES DEBUG_POSTFIX "d")

target_include_directories(VRG3DBase PUBLIC ${G3D_INCLUDE_DIR} ${GLG3D_INCLUDE_DIR} ${MinVR_INCLUDE_DIR} ${MinVRG3D_INCLUDE_DIR})

target_link_libraries(VRG3DBase PUBLIC ${MinVR_LIBRARIES} ${G3D_LIBRARIES})