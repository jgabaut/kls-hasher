#!/bin/sh -ex

CC="gcc"
SOURCE_DIR="src"
MAIN_SOURCE="$SOURCE_DIR/main.c"
ANVIL_SOURCE="$SOURCE_DIR/anvil__kls_hasher.c"
BIN="kls_hasher"

"$CC" "$MAIN_SOURCE" "$ANVIL_SOURCE" -DKOLISEO_HAS_GULP -lkoliseo -lncurses -o "$BIN"
