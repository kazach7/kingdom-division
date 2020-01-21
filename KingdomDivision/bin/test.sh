#!/bin/sh

BEFORE_TIME=$(date +%s%N)
./kingdom-division $1
AFTER_TIME=$(date +%s%N)
ELAPSED=$((AFTER_TIME - BEFORE_TIME))
echo "elapsed: $((ELAPSED/1000000)) milliseconds"

