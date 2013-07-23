#!/bin/sh

valgrind --leak-check=full --show-reachable=yes --gen-suppressions=all ./vinterm 2>&1 >/dev/null | grep -v "=.*" > vinterm.supp
