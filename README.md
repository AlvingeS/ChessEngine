# Setup

1. Update submodules
```bash
git submodule update --init --recursive
```

2. Install dependencies
```bash
sudo apt update && sudo apt install cmake build-essential stockfish qtbase5-dev qttools5-dev libqt5svg5-dev
```

3. Download opening book and place into `scripts/` folder
   - https://github.com/official-stockfish/books/blob/master/8moves_v3.pgn.zip

4. Install cutechess (needed for SPRT testing, Elo tests, and playing against the engine)
   - Clone repo from https://github.com/cutechess/cutechess
   - Build it:
   ```bash
   cd cutechess && mkdir build && cd build && cmake .. && make
   ```
   - Add cutechess-cli to PATH:
   ```bash
   echo 'export PATH=$PATH:~/repos/cutechess/build' >> ~/.bashrc && source ~/.bashrc
   ```

5. Add the following aliases to your `~/.bashrc` (see **Aliases** section below)

---

# Aliases

Paste these into your `~/.bashrc`:

```bash
# Git
alias gs='git status'
alias gl='git log'
alias gb='git branch'
alias gcane='git add . && git commit --amend --no-edit'

# Build
alias bld='cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug'
alias bldr='cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release'
alias mk='cmake --build build/debug -j 12'
alias mkr='cmake --build build/release -j 12'

# Run
alias run='./build/debug/executables/main'
alias runr='./build/release/executables/main'

# Test
alias tst='GTEST_COLOR=1 ctest --test-dir build/debug --output-on-failure -j12'
alias tstr='GTEST_COLOR=1 ctest --test-dir build/release --output-on-failure -j12'

# Tools
alias run_hyper='hyperfine --warmup 10 --min-runs 50 "./build/release/executables/main bench"'
alias profile='valgrind --tool=callgrind ./build/debug/executables/main'
alias cutechess='~/repos/cutechess/build/cutechess'
```

---

# Tools

| Command | Description |
|---|---|
| `run_hyper` | Benchmark with hyperfine (50 runs, 10 warmup) |
| `profile` | Generate callgrind profile (`callgrind.out.<pid>`) |
| `kcachegrind callgrind.out.<pid>` | Visualize callgrind profile |
| `python dump_repo.py` | Dump repo to markdown for AI analysis |

---

# Testing

**SPRT test** — test current build against a saved binary:
```bash
./scripts/sprt_test.sh <name_of_binary>
```

**Elo test** — test current build against Stockfish at a given Elo:
```bash
./scripts/elo_test.sh <elo> # min 1320
```