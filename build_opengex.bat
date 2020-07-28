mkdir 3rdparty\build\opengex
pushd 3rdparty\build\opengex
rm -rf *
cmake -DCMAKE_INSTALL_PREFIX=..\..\opengex  ..\..\opengex
cmake --build . --config debug --target install
popd
