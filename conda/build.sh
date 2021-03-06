#!/usr/bin/env bash

mkdir build && cd build
if [ `uname` == Darwin ]; then
    ${BUILD_PREFIX}/bin/cmake \
        .. \
        -DCMAKE_INSTALL_PREFIX=${PREFIX} \
        -DCMAKE_OSX_SYSROOT=${CONDA_BUILD_SYSROOT} \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=${CLANG} \
        -DCMAKE_C_COMPILER=${CLANGXX} \
        -DCMAKE_C_FLAGS="${CFLAGS} ${OPTS}" \
        -DCMAKE_CXX_FLAGS="${CXXFLAGS} ${OPTS}" \
        -DCMAKE_VERBOSE_MAKEFILE=TRUE \
        -DBUILD_TESTS=TRUE \
        -DBUILD_PYTHON_BINDINGS=TRUE \
        -DPYTHON_EXECUTABLE=${PYTHON} \
        -DPYTHON_LIBRARY=${STDLIB_DIR}/libpython${PY_VER}.a
fi
if [ `uname` == Linux ]; then
    ${BUILD_PREFIX}/bin/cmake \
        .. \
        -DCMAKE_INSTALL_PREFIX=${PREFIX} \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_C_COMPILER=${GCC} \
        -DCMAKE_C_COMPILER=${GXX} \
        -DCMAKE_C_FLAGS="${CFLAGS} ${OPTS}" \
        -DCMAKE_CXX_FLAGS="${CXXFLAGS} ${OPTS}" \
        -DCMAKE_VERBOSE_MAKEFILE=TRUE \
        -DBUILD_TESTS=TRUE \
        -DBUILD_TESTS=TRUE \
        -DBUILD_PYTHON_BINDINGS=TRUE \
        -DPYTHON_EXECUTABLE=${PYTHON} \
        -DPYTHON_LIBRARY=${STDLIB_DIR}/libpython${PY_VER}.a
fi

make -j${CPU_COUNT} && env CTEST_OUTPUT_ON_FAILURE=1 make test && make install
