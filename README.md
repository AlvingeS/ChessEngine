# Project Setup
### Initialize submodules
```bash
git submodule update --init --recursive
```
---
# Bash Aliases
Add these to your shell config (e.g. `~/.bashrc`), and run them from the project root:
```bash
alias gs='git status'
alias gl='git log'
alias gcane='git add . && git commit --amend --no-edit'
alias gb='git branch'
alias run='./build/debug/executables/main'
alias runr='./build/release/executables/main'
alias run_hyper='hyperfine --warmup 10 --min-runs 50 ./build/release/executables/main'
alias mk='cmake --build build/debug -j 12'
alias mkr='cmake --build build/release -j 12'
alias bld='cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug'
alias bldr='cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release'
alias tst='GTEST_COLOR=1 ctest --test-dir build/debug --output-on-failure -j12'
alias tstr='GTEST_COLOR=1 ctest --test-dir build/release --output-on-failure -j12'
alias count_lines='find . -name "*.cpp" -o -name "*.h" -o -name "*.py" | xargs wc -l'
alias profile='valgrind --tool=callgrind ./build/debug/executables/main'
```
---

### Get stockfish (used to test against in perft results)
```bash
sudo apt-get install stockfish