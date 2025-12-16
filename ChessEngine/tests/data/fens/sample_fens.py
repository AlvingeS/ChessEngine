#!/usr/bin/env python3
# sample_fens.py

import csv
import os
import random
from collections import defaultdict

# -----------------------------
# CONFIG (edit these)
# -----------------------------
INPUT_CSV = "fen_dataset.csv"   # in same folder as this script
OUTPUT_PREFIX = "sampled"       # sampled_1000.csv etc

N_SAMPLES = 100
SEED = 1

MIN_EP_FRAC = 0.15      # best-effort fraction with EP target present
MIN_HM49_FRAC = 0.05    # best-effort fraction with halfmove == 49
# -----------------------------


def parse_fen(fen: str):
    fen = fen.strip()
    parts = fen.split()
    if len(parts) < 4:
        raise ValueError(f"Bad FEN (too few fields): {fen!r}")

    placement = parts[0]
    stm = parts[1] if len(parts) > 1 else "w"
    castling = parts[2] if len(parts) > 2 else "-"
    ep = parts[3] if len(parts) > 3 else "-"
    halfmove = int(parts[4]) if len(parts) > 4 and parts[4].isdigit() else 0
    fullmove = int(parts[5]) if len(parts) > 5 and parts[5].isdigit() else 1

    piece_count = sum(1 for c in placement if c.isalpha())

    rights = 0 if castling == "-" else len(castling)
    w_castle = int(("K" in castling) or ("Q" in castling))
    b_castle = int(("k" in castling) or ("q" in castling))
    ep_present = int(ep != "-")
    hm49 = int(halfmove == 49)

    return {
        "fen": fen,
        "piece_count": piece_count,
        "stm": stm,
        "castling": castling,
        "rights": rights,
        "w_castle": w_castle,
        "b_castle": b_castle,
        "ep_present": ep_present,
        "halfmove": halfmove,
        "fullmove": fullmove,
        "hm49": hm49,
    }


def piece_bin(pc: int):
    if pc >= 26:
        return "many"
    if pc >= 15:
        return "mid"
    return "few"


def build_stratum(r):
    # keep it simple but useful for variety
    return (
        f"{r['piece_bin']}"
        f"|c{r['rights']}"
        f"|w{r['w_castle']}"
        f"|b{r['b_castle']}"
        f"|ep{r['ep_present']}"
        f"|hm49{r['hm49']}"
        f"|stm{r['stm']}"
    )


def sample_from_list(lst, k, rng):
    if k <= 0:
        return []
    if len(lst) <= k:
        return list(lst)
    return rng.sample(lst, k)


def read_fens_from_csv(path: str):
    """
    Accepts either:
      - CSV with a 'FEN' header column, or
      - CSV with no header, where first column is the FEN string.
    """
    fens = []
    with open(path, "r", encoding="utf-8", newline="") as f:
        # peek first row
        sample = f.read(4096)
        f.seek(0)

        has_header = "FEN" in sample.splitlines()[0].split(",")
        if has_header:
            reader = csv.DictReader(f)
            for row in reader:
                fen = (row.get("FEN") or "").strip()
                if fen:
                    fens.append(fen)
        else:
            reader = csv.reader(f)
            for row in reader:
                if not row:
                    continue
                fen = (row[0] or "").strip()
                if fen and fen.lower() != "fen":
                    fens.append(fen)

    return fens


def main():
    rng = random.Random(SEED)

    script_dir = os.path.dirname(os.path.abspath(__file__))
    in_path = os.path.join(script_dir, INPUT_CSV)

    raw_fens = read_fens_from_csv(in_path)

    rows = []
    bad = 0
    for fen in raw_fens:
        try:
            rows.append(parse_fen(fen))
        except Exception:
            bad += 1

    if not rows:
        raise SystemExit("No valid FEN rows parsed.")

    for r in rows:
        r["piece_bin"] = piece_bin(r["piece_count"])
        r["stratum"] = build_stratum(r)

    n = min(N_SAMPLES, len(rows))

    # ---- quotas (best-effort) ----
    want_ep = int(round(MIN_EP_FRAC * n))
    want_hm49 = int(round(MIN_HM49_FRAC * n))

    ep_rows = [r for r in rows if r["ep_present"] == 1]
    hm49_rows = [r for r in rows if r["hm49"] == 1]

    # cap by availability
    want_ep = min(want_ep, len(ep_rows))
    want_hm49 = min(want_hm49, len(hm49_rows))

    selected = []
    selected_set = set()

    def add_rows(picks):
        for r in picks:
            if r["fen"] not in selected_set:
                selected.append(r)
                selected_set.add(r["fen"])

    # 1) force HM49 quota
    add_rows(sample_from_list(hm49_rows, want_hm49, rng))

    # 2) force EP quota (after hm49 so we can overlap naturally)
    add_rows(sample_from_list(ep_rows, want_ep, rng))

    # 3) fill rest via stratified sampling favoring rare strata
    remaining = n - len(selected)
    if remaining > 0:
        strata_to_rows = defaultdict(list)
        for r in rows:
            if r["fen"] not in selected_set:
                strata_to_rows[r["stratum"]].append(r)

        strata = list(strata_to_rows.keys())
        sizes = {s: len(strata_to_rows[s]) for s in strata}
        weights = [1.0 / (sizes[s] ** 0.7) for s in strata]

        while remaining > 0 and strata:
            s = rng.choices(strata, weights=weights, k=1)[0]
            bucket = strata_to_rows[s]
            r = bucket.pop(rng.randrange(len(bucket)))
            selected.append(r)
            selected_set.add(r["fen"])
            remaining -= 1
            if not bucket:
                j = strata.index(s)
                strata.pop(j)
                weights.pop(j)

    out_path = os.path.join(script_dir, f"{OUTPUT_PREFIX}_{len(selected)}.csv")

    # Output: CSV with NO header, just one FEN per row
    with open(out_path, "w", encoding="utf-8", newline="") as f:
        w = csv.writer(f)
        for r in selected:
            w.writerow([r["fen"]])

    print(f"Read {len(raw_fens)} rows from {INPUT_CSV}. Parsed {len(rows)} valid FENs (skipped {bad} bad).")
    print(f"Requested n={N_SAMPLES}, wrote n={len(selected)} -> {os.path.basename(out_path)}")
    print(f"Quota achieved (best-effort): hm49={sum(r['hm49'] for r in selected)}, ep={sum(r['ep_present'] for r in selected)}")


if __name__ == "__main__":
    main()
