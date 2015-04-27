# Install script for directory: /Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil/libSOIL.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSOIL.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSOIL.a")
    execute_process(COMMAND "/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libSOIL.a")
  endif()
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/SOIL" TYPE FILE FILES
    "/Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil/src/image_DXT.h"
    "/Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil/src/image_helper.h"
    "/Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil/src/SOIL.h"
    "/Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil/src/stb_image_aug.h"
    "/Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil/src/stbi_DDS_aug.h"
    "/Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil/src/stbi_DDS_aug_c.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

file(WRITE "/Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil/${CMAKE_INSTALL_MANIFEST}" "")
foreach(file ${CMAKE_INSTALL_MANIFEST_FILES})
  file(APPEND "/Users/sanchitgarg/Desktop/cggt/spring 2015/phys based ani/cis563_FinalProject/code/paanipuri/paanipuri/thirdparty/soil/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
endforeach()
