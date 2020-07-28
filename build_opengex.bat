mkdir 3rdparty\build\opengex
pushd 3rdparty\build\opengex
rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=..\..\ -G "Visual Studio 15 2017 Win64" -Thost=x64 ..\..\src\opengex
cmake --build . --config debug --target install
popd
