VALSORT_PATH=~ec2-user/gensort/32/valsort
for i in {32..126}; do cat  $1_${i}_0 $1_${i}_1 >> $1; done
${VALSORT_PATH} $1

