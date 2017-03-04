mkdir -p build_ch03
(cd build_ch03; \
 cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../ch03; \
 make)
