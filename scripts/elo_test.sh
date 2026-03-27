#!/bin/bash

# Usage: ./scripts/elo_test.sh <elo>
# Tests current build against Stockfish at the specified Elo (1320-3190)
# Example: ./scripts/elo_test.sh 1320
# Requires: cutechess-cli and stockfish on PATH

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OPENINGS="$SCRIPT_DIR/8moves_v3.pgn"

ENGINE="build/release/executables/main"

# --- Validate input ---
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <elo>"
    echo "Example: $0 1320"
    exit 1
fi

if ! [[ "$1" =~ ^[0-9]+$ ]]; then
    echo "Error: Elo must be a number."
    exit 1
fi

if [ "$1" -lt 1320 ] || [ "$1" -gt 3190 ]; then
    echo "Error: Elo must be between 1320 and 3190 (Stockfish's supported range)."
    exit 1
fi

if [ ! -f "$ENGINE" ]; then
    echo "Error: engine binary '$ENGINE' not found. Did you build first?"
    exit 1
fi

if ! command -v cutechess-cli &> /dev/null; then
    echo "Error: cutechess-cli not found in PATH."
    echo "Once built, add it to your PATH by running:"
    echo "  echo 'export PATH=\$PATH:/path/to/cutechess/build' >> ~/.bashrc && source ~/.bashrc"
    echo "Or symlink it: sudo ln -s /path/to/cutechess/build/cutechess-cli /usr/local/bin/cutechess-cli"
    exit 1
fi

if ! command -v stockfish &> /dev/null; then
    echo "Error: stockfish not found in PATH."
    exit 1
fi

echo "========================================"
echo "  Elo Test"
echo "  Engine:   $ENGINE"
echo "  Opponent: Stockfish @ $1 Elo"
echo "  Games:    100"
echo "========================================"

cutechess-cli \
    -engine cmd="$ENGINE" proto=uci name="Smorklaren" \
    -engine cmd=stockfish proto=uci name="SF_$1" \
        option.UCI_LimitStrength=true option.UCI_Elo="$1" \
    -each tc=10+0.1 \
    -openings file="$OPENINGS" format=pgn order=random \
    -games 100 -repeat -recover \
    -ratinginterval 10