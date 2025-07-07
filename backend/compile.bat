@echo off
setlocal

g++ -shared -o libsuffixarray.dll suffix_array.cpp -static -lstdc++ -D__STDC_WANT_LIB_EXT1__=1

endlocal