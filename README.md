# libztime ver. 0.0.1
Time management C library

# Configure as shared library:
cmake .

# Configure as static library:
cmake . -DBUILD_SHARED_LIBS=OFF

# Configure with debug info:
cmake . -DCMAKE_BUILD_TYPE=Debug

# Compile library:
cmake --build .

# Compile library and sample:
cmake --build . --target sample
