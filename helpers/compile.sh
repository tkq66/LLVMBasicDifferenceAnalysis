clang++  -o bin/TaintPass src/TaintPass.cpp `llvm-config --cxxflags` `llvm-config --ldflags` `llvm-config --libs` -lpthread -lncurses -ldl -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1