project(VRG3DBase)

#include(AutoBuildG3D)
#AutoBuild_use_package_MinVRG3D(${PROJECT_NAME} PUBLIC)

find_package(MinVR REQUIRED HINTS ${MinVR_DIR}) # MinVR_INCLUDE_DIR, MinVR_LIBRARIES
message(error ${MinVR_DIR})
#---------------------- Find & Add Dependencies ----------------------
message(STATUS "Init VRG3DBase build")

set(EXTERNAL_CMAKE_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(HEADERFILES 
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
  ${HEADERFILES}
)


if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  # Windows specific code
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_HAS_AUTO_PTR_ETC=1)
  add_definitions(-DUNICODE -D_UNICODE)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")

set_target_properties(VRG3DBase PROPERTIES DEBUG_POSTFIX "d")

#target_include_directories(VRG3DBase PUBLIC ${G3D_INCLUDE_DIR} ${GLG3D_INCLUDE_DIR} ${MinVR_INCLUDE_DIR} ${MinVRG3D_INCLUDE_DIR})

target_link_libraries(VRG3DBase PUBLIC MinVR::MinVR MinVR::MinVR_G3D)

install(FILES ${HEADERFILES} DESTINATION include/${PROJECT_NAME})

INSTALL(TARGETS VRG3DBase
   ARCHIVE DESTINATION lib
   LIBRARY DESTINATION lib
   RUNTIME DESTINATION bin
   )