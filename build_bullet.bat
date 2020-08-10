@echo off
mkdir 3rdparty\build\bullet
pushd 3rdparty\build\bullet
rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=..\..\bullet -DCMAKE_INSTALL_RPATH=..\..\bullet -DINSTALL_LIBS=ON -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON -DBUILD_SHARED_LIBS=OFF -DBUILD_BULLET2_DEMOS=OFF -DBUILD_OPENGL3_DEMOS=OFF -DBUILD_CPU_DEMOS=OFF -DBUILD_PYBULLET=OFF -DUSE_DOUBLE_PRECISION=ON -DBUILD_EXTRAS=OFF -DBUILD_UNIT_TESTS=OFF -G "Visual Studio 15 2017 Win64" -Thost=x64 ..\..\bullet
cmake --build . --config debug --target install
popd