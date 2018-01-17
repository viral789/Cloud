#Number of Nodes
nodes=8

#Send generate_raid to all 8 slave nodes
for i in `seq 1 $nodes`
do
	scp -r generate_raid.sh hadoop-slave-$i:/home/ubuntu
done


