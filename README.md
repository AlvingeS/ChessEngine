# Template

Bash aliases to be ran from root folder:
alias run='./build/executables/main'
alias mk='cmake --build build -j 12'
alias bld='cmake -S . -B build'
alias tst='GTEST_COLOR=1 ctest --test-dir ./build --output-on-failure -j12'

Add the following bash