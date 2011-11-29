export EC2_HOME=/home/vineet/Downloads/ec2-api-tools-1.5.0.0
export PATH=$PATH:/home/vineet/Downloads/ec2-api-tools-1.5.0.0/bin
export EC2_PRIVATE_KEY=../pk-UJ7CMV7AI4XUS2BWKBTZNWAOXYVTXROY.pem
export EC2_CERT=../cert-UJ7CMV7AI4XUS2BWKBTZNWAOXYVTXROY.pem
export EC2_CERT=/home/vineet/Downloads/cert-UJ7CMV7AI4XUS2BWKBTZNWAOXYVTXROY.pem
export EC2_PRIVATE_KEY=/home/vineet/Downloads/pk-UJ7CMV7AI4XUS2BWKBTZNWAOXYVTXROY.pem
export EC2_URL=https://ec2.us-east-1.amazonaws.com

#Launches a High CPU extra large instance with all four ephemeral drives loaded.
#ec2-run-instances ami-1b814f72 -b /dev/sdb=ephemeral0 -b /dev/sdc=ephemeral1 -b /dev/sdd=ephemeral2 -b /dev/sde=ephemeral3 -t c1.xlarge -k vineet
ec2-run-instances ami-31814f58 -b /dev/sdb=ephemeral0 -t m1.small -k vineet
