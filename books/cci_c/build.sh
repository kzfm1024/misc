mkdir -p build
(cd build ; \
 cmake -DCMAKE_BUILD_TYPE=Debug -G"Eclipse CDT4 - Unix Makefiles" ../utf; \
 make)
