#!/bin/bash

# Usage: ./scripts/sprt.sh <binary_name>
# Tests current build against a saved binary under binaries/
# Example: ./scripts/sprt.sh smorklaren_v1
# Requires: cutechess-cli on PATH

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(dirname "$SCRIPT_DIR")"

ENGINE_NEW="$ROOT_DIR/build/release/executables/main"
BINARIES_DIR="$ROOT_DIR/binaries"
OPENINGS="$SCRIPT_DIR/8moves_v3.pgn"

# --- Validate input ---
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <binary_name>"
    echo "Example: $0 smorklaren_v1"
    exit 1
fi

BASELINE="$BINARIES_DIR/$1"

if [ ! -f "$BASELINE" ]; then
    echo "Error: baseline binary '$BASELINE' not found."
    echo "Available binaries:"
    ls "$BINARIES_DIR" 2>/dev/null || echo "  (none)"
    exit 1
fi

if [ ! -f "$ENGINE_NEW" ]; then
    echo "Error: engine binary '$ENGINE_NEW' not found. Did you build first?"
    exit 1
fi

if [ ! -f "$OPENINGS" ]; then
    echo "Error: opening book '$OPENINGS' not found."
    echo "Download 8moves_v3.pgn from the Stockfish repo and place it at $OPENINGS"
    exit 1
fi

if ! command -v cutechess-cli &> /dev/null; then
    echo "Error: cutechess-cli not found in PATH."
    echo "Once built, add it to your PATH by running:"
    echo "  echo 'export PATH=\$PATH:/path/to/cutechess/build' >> ~/.bashrc && source ~/.bashrc"
    echo "Or symlink it: sudo ln -s /path/to/cutechess/build/cutechess-cli /usr/local/bin/cutechess-cli"
    exit 1
fi

echo "========================================"
echo "  SPRT Test"
echo "  New:      $ENGINE_NEW"
echo "  Baseline: $BASELINE"
echo "  elo0=0 elo1=10 alpha=0.05 beta=0.05"
echo "========================================"

cutechess-cli \
    -engine cmd="$ENGINE_NEW" name="new" proto=uci \
    -engine cmd="$BASELINE" name="$1" proto=uci \
    -each tc=3+0.2 \
    -openings file="$OPENINGS" format=pgn order=random \
    -games 2 -rounds 2000 -repeat \
    -recover -concurrency 4 \
    -sprt elo0=0 elo1=10 alpha=0.05 beta=0.05 \
    -ratinginterval 50