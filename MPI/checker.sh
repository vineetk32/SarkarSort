#!/bin/bash
VALSORT_PATH=~/gensort/64/valsort
for i in {32..126}; do cat $1.sorted.${i} >> $1; done
${VALSORT_PATH} $1

