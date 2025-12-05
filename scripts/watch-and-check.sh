#!/usr/bin/env bash
# Watch source files and run `make check` on changes.
# Requires inotifywait (inotify-tools) to be installed.

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
WATCH_DIRS=("$ROOT/src" "$ROOT/includes" "$ROOT/libft" "$ROOT/tests")

if ! command -v inotifywait >/dev/null 2>&1; then
  echo "inotifywait not found. Install inotify-tools or run 'make check' manually."
  exit 2
fi

echo "Watching: ${WATCH_DIRS[*]}"
while true; do
  inotifywait -e modify,create,delete -r ${WATCH_DIRS[*]} >/dev/null 2>&1
  echo "Change detected — running: make check"
  (cd "$ROOT" && make check) || echo "make check failed"
  echo "Waiting for changes..."
done
