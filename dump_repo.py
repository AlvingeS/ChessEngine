#!/usr/bin/env python3
import os
from pathlib import Path

ROOT = Path("ChessEngine").resolve()

EXCLUDE_DIRS = {
    ".git",
    ".idea",
    ".vscode",
    "build",
    "cmake-build-debug",
    "cmake-build-release",
    "__pycache__",
    "dist",
    "out",
}

EXCLUDE_EXT = {
    ".o",
    ".obj",
    ".a",
    ".so",
    ".dll",
    ".exe",
    ".png",
    ".jpg",
    ".jpeg",
    ".gif",
    ".ico",
    ".pdf",
    ".zip",
    ".tar",
    ".gz",
    ".7z",
}

OUTPUT = "repo_dump.md"


def is_excluded_dir(d: Path) -> bool:
    return d.name in EXCLUDE_DIRS


def is_excluded_file(f: Path) -> bool:
    return f.suffix in EXCLUDE_EXT


def write_tree(out):
    out.write("REPO STRUCTURE\n")
    for root, dirs, files in os.walk(ROOT):
        root_path = Path(root)
        dirs[:] = [d for d in dirs if not is_excluded_dir(root_path / d)]

        rel_root = root_path.relative_to(ROOT)
        indent_level = len(rel_root.parts)
        indent = "  " * indent_level
        name = "." if rel_root == Path(".") else rel_root.name
        out.write(f"{indent}{name}/\n")

        for f in sorted(files):
            if is_excluded_file(root_path / f):
                continue
            out.write(f"{indent}  {f}\n")


def guess_lang(path: Path) -> str:
    ext = path.suffix
    return {
        ".cpp": "cpp",
        ".cc": "cpp",
        ".cxx": "cpp",
        ".hpp": "cpp",
        ".h": "cpp",
        ".c": "c",
        ".py": "python",
        ".rs": "rust",
        ".java": "java",
        ".js": "javascript",
        ".ts": "typescript",
        ".cmake": "cmake",
        ".txt": "",
        ".md": "md",
    }.get(ext, "")


def count_lines(text: str) -> int:
    if not text:
        return 0
    # Count newline characters; if file doesn't end with newline, add one line
    lines = text.count("\n")
    if not text.endswith("\n"):
        lines += 1
    return lines


def write_files(out, stats):
    code_exts = {".h", ".cpp", ".inl"}

    for root, dirs, files in os.walk(ROOT):
        root_path = Path(root)
        dirs[:] = [d for d in dirs if not is_excluded_dir(root_path / d)]

        for fname in sorted(files):
            path = root_path / fname
            if is_excluded_file(path):
                continue

            rel = path.relative_to(ROOT)
            lang = guess_lang(path)
            ext = path.suffix

            out.write("\n\n--------------------\n\n")
            out.write(f"# FILE: {rel.as_posix()}\n\n")
            out.write(f"```{lang}\n")
            try:
                text = path.read_text(encoding="utf-8", errors="replace")
            except Exception as e:
                out.write(f"<<unable to read file: {e}>>\n")
                text = ""
            else:
                out.write(text)
                if not text.endswith("\n"):
                    out.write("\n")
            out.write("```\n")

            # Collect stats for code files
            if ext in code_exts:
                n_lines = count_lines(text)
                stats["total_code_files"] += 1
                stats["total_code_lines"] += n_lines
                stats["per_ext"][ext]["files"] += 1
                stats["per_ext"][ext]["lines"] += n_lines


def print_stats(stats):
    print("\n=== Code stats (.h, .cpp, .inl) ===")
    print(f"Total code files: {stats['total_code_files']}")
    print(f"Total code lines: {stats['total_code_lines']}")
    print("By extension:")
    for ext in [".h", ".cpp", ".inl"]:
        data = stats["per_ext"][ext]
        print(
            f"  {ext:4} -> files: {data['files']:5d}, "
            f"lines: {data['lines']:7d}"
        )
    print("===================================")


def main():
    stats = {
        "total_code_files": 0,
        "total_code_lines": 0,
        "per_ext": {
            ".h": {"files": 0, "lines": 0},
            ".cpp": {"files": 0, "lines": 0},
            ".inl": {"files": 0, "lines": 0},
        },
    }

    with open(OUTPUT, "w", encoding="utf-8") as out:
        write_tree(out)
        write_files(out, stats)

    print(f"Wrote {OUTPUT}")
    print_stats(stats)


if __name__ == "__main__":
    main()
