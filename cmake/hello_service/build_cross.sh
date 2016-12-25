TOOLCHAIN_FILE=`pwd`/rostoolchain-aarch64.cmake

mkdir -p build_cross
(cd build_cross; \
 cmake -G"Eclipse CDT4 - Unix Makefiles" \
       -DCMAKE_BUILD_TYPE=Debug \
       -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE \
       ../src; \
 make; \
 make install)
