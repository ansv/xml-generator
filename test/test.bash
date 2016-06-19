#!/bin/bash

gcc xmlgen.c -o xmlgen
cpp cfg.txt |grep -v "^\s*#" |./xmlgen >cfg.xml
