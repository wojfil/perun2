cmake -S .. -B . -G "MinGW Makefiles" -DUSED_CPP_STANDARD=17
cmake --build .
perun2.exe
pause