#############################################################
#
# An example for using the gumstix arm-linux toolchain is below.
# Copy these lines to $(ROS_ROOT)/rostoolchain.cmake to try them out.
#
#set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_C_COMPILER /opt/arm-linux/bin/arm-linux-gcc)
#set(CMAKE_CXX_COMPILER /opt/arm-linux/bin/arm-linux-g++)
#set(CMAKE_FIND_ROOT_PATH /opt/arm-linux)
# Have to set this one to BOTH, to allow CMake to find rospack
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

#############################################################
# Cross-compilation loader
#
# This file is passed to cmake on the command line via
# -DCMAKE_TOOLCHAIN_FILE.

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(GNU_MACHINE "aarch64-linux-gnu" CACHE STRING "GNU compiler triple")

# specify the cross compiler
set(CMAKE_C_COMPILER   /usr/bin/aarch64-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++)
set(CMAKE_PKG_CONFIG /usr/bin/aarch64-linux-gnu-pkg-config)

set(CMAKE_CXX_FLAGS           "" CACHE INTERAL "")
set(CMAKE_C_FLAGS             "" CACHE INTERAL "")
set(CMAKE_SHARED_LINKER_FLAGS "" CACHE INTERAL "")
set(CMAKE_MODULE_LINKER_FLAGS "" CACHE INTERAL "")
set(CMAKE_EXE_LINKER_FLAGS    "" CACHE INTERAL "")

# for ARM64 Build
set(ARM_LINKER_FLAGS "-Wl,--no-undefined -Wl,--gc-sections -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now")
set(CMAKE_SHARED_LINKER_FLAGS "${ARM_LINKER_FLAGS} ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${ARM_LINKER_FLAGS} ${CMAKE_MODULE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS    "${ARM_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS}")

# where is the target environment
set(CMAKE_FIND_ROOT_PATH  /tftpboot/bdk-arm64)
set(CMAKE_INSTALL_PREFIX /opt/ros/kinetic)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/tftpboot/bdk-arm64/usr/lib/aarch64-linux-gnu")


# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set(CATKIN_ENABLE_TESTING OFF)

set(CMAKE_LIBRARY_PATH "/tftpboot/bdk-arm64/lib" "/tftpboot/bdk-arm64/usr/lib" "/tftpboot/bdk-arm64/usr/lib/aarch64-linux-gnu")
set(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} /tftpboot/bdk-arm64/usr/lib /tftpboot/bdk-arm64/usr/lib/aarch64-linux-gnu)
