#!/bin/bash

gcc xmlgen.c -o xmlgen
cpp "$1" |grep -v "^\s*#" |./xmlgen >"$1.xml"
