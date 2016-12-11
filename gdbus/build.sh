mkdir -p build
(cd src; ./codegen.sh)
(cd build; \
 cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug ../src; \
 make)
