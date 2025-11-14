# Project Setup
### Initialize submodules
```bash
git submodule update --init --recursive
```
---
# Bash Aliases
Add these to your shell config (e.g. `~/.bashrc`), and run them from the project root:
```bash
alias run='./build/debug/executables/main'
alias runr='./build/release/executables/main'
alias run_valgrind='valgrind ./build/debug/executables/main --track-origin=yes'
alias run_hyper='hyperfine --warmup 5 --runs 20 ./build/release/executables/main'
alias mk='cmake --build build/debug -j 12'
alias mkr='cmake --build build/release -j 12'
alias bld='cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug'
alias bldr='cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release'
alias tst='(cd build/debug && GTEST_COLOR=1 ctest --output-on-failure -j12)'
alias tstr='(cd build/release && GTEST_COLOR=1 ctest --output-on-failure -j12)'
alias count_lines='find . -name "*.cpp" -o -name "*.h" -o -name "*.py" | xargs wc -l'
alias profile='valgrind --tool=callgrind ./build/debug/executables/main'
```
---

### Get stockfish (used to test against in perft results)
```bash
sudo apt-get install stockfish