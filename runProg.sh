#!/bin/bash

INPUT_DEV=$(cat /proc/bus/input/devices | grep "kbd" | awk '{print $4}')

INPUT_DEV="/dev/input/""$INPUT_DEV"

./main "$INPUT_DEV"
