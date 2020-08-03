mkdir 3rdparty\build\opengex
pushd 3rdparty\build\opengex
rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=..\..\opengex -G "Visual Studio 15 2017 Win64" -Thost=x64 ..\..\opengex
cmake --build . --config debug --target install
popd
