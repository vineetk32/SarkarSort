#!/bin/bash

for ((i=0; i < 1000000000; i = i+10000000));
do
	sudo ./gensort -a -b$i  10000000 /media/ephemeral0/input_${i}
	
done
