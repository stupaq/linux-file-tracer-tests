#!/usr/bin/env bash

[[ $# -eq 2 ]] || exit 1
diff <(tail -n+3 $1 | cut -d " " -f 2-) <(tail -n+3 $2 | cut -d " " -f 2-)
