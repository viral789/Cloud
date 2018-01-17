#install mdadm
apt-get install mdadm
#create Raid 0 of 3 combine devices with name NEW_RAID
mdadm --create /dev/md0 --run --level=0 --name=NEW_RAID --raid-devices=3 /dev/nvme0n1 /dev/xvdc /dev/xvda
mkfs.ext4 -L NEW_RAID /dev/md0
mkdir -p /mnt/raid
#Create a mount point
mount LABEL=NEW_RAID /mnt/raid
#give permission 
chmod 777 /mnt/raid
