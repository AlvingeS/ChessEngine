#!/usr/bin/env python3
# sample_fens.py

import csv
import os
import random
from collections import defaultdict

# -----------------------------
# CONFIG
# -----------------------------
INPUT_CSV = "fen_dataset.csv"
OUTPUT_PREFIX = "sampled"

N_SAMPLES_LIST = [150, 1000]
NUM_VARIANTS = 2

SEED = 1

MIN_EP_FRAC = 0.15
MIN_CASTLE_FLAG_FRAC = 0.05

PIECE_BUCKETS = [
    (0, 8, "p0_8"),
    (9, 16, "p9_16"),
    (17, 24, "p17_24"),
    (25, 32, "p25_32"),
]


# -----------------------------
# UTIL
# -----------------------------

def progress(msg):
    print(msg, flush=True)


def parse_fen(fen: str):
    parts = fen.split()

    placement = parts[0]
    stm = parts[1]
    castling = parts[2]
    ep = parts[3]

    piece_count = sum(1 for c in placement if c.isalpha())

    has_K = int("K" in castling)
    has_Q = int("Q" in castling)
    has_k = int("k" in castling)
    has_q = int("q" in castling)

    return {
        "fen": fen,
        "piece_count": piece_count,
        "stm": stm,
        "castling": castling,
        "has_K": has_K,
        "has_Q": has_Q,
        "has_k": has_k,
        "has_q": has_q,
        "ep_present": int(ep != "-"),
    }


def piece_bucket(pc: int):
    for lo, hi, name in PIECE_BUCKETS:
        if lo <= pc <= hi:
            return name
    return "other"


def build_stratum(r):
    return (
        f"{r['piece_bucket']}"
        f"|{r['stm']}"
        f"|{r['castling']}"
        f"|ep{r['ep_present']}"
    )


def read_fens_from_csv(path):
    fens = []

    with open(path, "r", encoding="utf-8") as f:

        sample = f.read(1024)
        f.seek(0)

        has_header = "FEN" in sample.splitlines()[0]

        if has_header:

            reader = csv.DictReader(f)

            for row in reader:
                fen = row["FEN"].strip()
                if fen:
                    fens.append(fen)

        else:

            reader = csv.reader(f)

            for row in reader:
                if row:
                    fens.append(row[0].strip())

    return fens


def variant_label(i):
    s = ""
    n = i

    while True:
        s = chr(ord("A") + n % 26) + s
        n = n // 26 - 1
        if n < 0:
            break

    return s


# -----------------------------
# SAMPLING
# -----------------------------

def weighted_fill(rows, selected_set, need, rng):

    strata = defaultdict(list)

    for r in rows:
        if r["fen"] not in selected_set:
            strata[r["stratum"]].append(r)

    keys = list(strata.keys())

    sizes = {k: len(strata[k]) for k in keys}

    weights = [1.0 / (sizes[k] ** 0.7) for k in keys]

    out = []

    step = 0

    while need > 0 and keys:

        step += 1

        if step % 50 == 0:
            progress(f"    weighted fill step {step} remaining={need}")

        k = rng.choices(keys, weights=weights, k=1)[0]

        bucket = strata[k]

        r = bucket.pop(rng.randrange(len(bucket)))

        out.append(r)

        need -= 1

        if not bucket:

            i = keys.index(k)

            keys.pop(i)
            weights.pop(i)

    return out


def stratified_sample(rows, n, rng):

    selected = []
    selected_set = set()

    def add(r):
        if r["fen"] not in selected_set:
            selected.append(r)
            selected_set.add(r["fen"])

    # -------------------------
    # feature quotas
    # -------------------------

    targets = {
        "ep_present": int(MIN_EP_FRAC * n),
        "has_K": int(MIN_CASTLE_FLAG_FRAC * n),
        "has_Q": int(MIN_CASTLE_FLAG_FRAC * n),
        "has_k": int(MIN_CASTLE_FLAG_FRAC * n),
        "has_q": int(MIN_CASTLE_FLAG_FRAC * n),
    }

    step = 0

    while True:

        step += 1

        if step % 50 == 0:
            progress(f"  feature step {step} selected={len(selected)}")

        deficits = {}

        for f, t in targets.items():

            cur = sum(r[f] for r in selected)

            deficits[f] = max(0, t - cur)

        unmet = {f: d for f, d in deficits.items() if d > 0}

        if not unmet:
            break

        if len(selected) >= n:
            break

        f = max(unmet, key=unmet.get)

        candidates = [
            r for r in rows
            if r["fen"] not in selected_set
            and r[f] == 1
        ]

        if not candidates:
            targets[f] = 0
            continue

        best = random.choice(candidates)

        add(best)

    # -------------------------
    # piece buckets
    # -------------------------

    progress("  piece bucket balancing")

    names = [b[2] for b in PIECE_BUCKETS]

    want = max(1, n // (2 * len(names)))

    for name in names:

        if len(selected) >= n:
            break

        cur = sum(
            1 for r in selected
            if r["piece_bucket"] == name
        )

        available = [
            r for r in rows
            if r["fen"] not in selected_set
            and r["piece_bucket"] == name
        ]

        need = max(0, want - cur)

        for r in random.sample(
            available,
            min(need, len(available))
        ):
            add(r)

    # -------------------------
    # fill rest
    # -------------------------

    remaining = n - len(selected)

    progress(f"  fill remaining {remaining}")

    if remaining > 0:

        for r in weighted_fill(
            rows,
            selected_set,
            remaining,
            rng,
        ):
            add(r)

    return selected


# -----------------------------
# MAIN
# -----------------------------

def main():

    rng = random.Random(SEED)

    script_dir = os.path.dirname(
        os.path.abspath(__file__)
    )

    path = os.path.join(
        script_dir,
        INPUT_CSV,
    )

    progress("Reading CSV")

    raw = read_fens_from_csv(path)

    rows = []

    for i, fen in enumerate(raw):

        try:
            rows.append(parse_fen(fen))
        except:
            pass

        if i % 50000 == 0 and i > 0:
            progress(f"parsed {i}")

    progress("Building strata")

    for r in rows:

        r["piece_bucket"] = piece_bucket(
            r["piece_count"]
        )

        r["stratum"] = build_stratum(r)

    for n_samples in N_SAMPLES_LIST:

        progress(f"\n=== n={n_samples} ===")

        needed = n_samples * NUM_VARIANTS

        if needed > len(rows):
            raise RuntimeError("not enough fens")

        available = list(rows)

        rng.shuffle(available)

        for v in range(NUM_VARIANTS):

            label = variant_label(v)

            progress(f"variant {label}")

            selected = stratified_sample(
                available,
                n_samples,
                rng,
            )

            selected_set = {
                r["fen"] for r in selected
            }

            out = os.path.join(
                script_dir,
                f"{OUTPUT_PREFIX}_{n_samples}_{label}.csv",
            )

            with open(
                out,
                "w",
                newline="",
                encoding="utf-8",
            ) as f:

                w = csv.writer(f)

                for r in selected:
                    w.writerow([r["fen"]])

            available = [
                r for r in available
                if r["fen"] not in selected_set
            ]

            progress(
                f"done {n_samples} {label}"
            )


if __name__ == "__main__":
    main()