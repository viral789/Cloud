##Add Private IP of all Slaves and Master in hosts
cd  /etc/
sudo chmod 777 hosts
sudo cat /home/ubuntu/ip-info >> hosts

#Generate RSA Key
ssh-keygen -t rsa -P ""

#Add Key to authorized keys
cat $HOME/.ssh/id_rsa.pub >> $HOME/.ssh/authorized_keys

#Send key, ip_info to all the slave nodes
nodes=8
for i in `seq 1 $nodes`
do
	scp -i /home/ubuntu/Hadoop-Multinode-Rushit.pem $HOME/.ssh/id_rsa.pub ubuntu@hadoop-slave-$i:/home/ubuntu/master.pub
	scp -i /home/ubuntu/Hadoop-Multinode-Rushit.pem /home/ubuntu/ip-info ubuntu@hadoop-slave-$i:/home/ubuntu/ip-info
	ssh -i /home/ubuntu/Hadoop-Multinode-Rushit.pem ubuntu@hadoop-slave-$i '
	sudo su <<EOF
	sudo cat /home/ubuntu/master.pub >> /root/.ssh/authorized_keys
	EOF'
done

# purge openjdk if installed to remove conflict  
sudo apt-get purge openjdk-\* -y  
sudo apt-get update -y  
sudo apt-get install python-software-properties -y  
sudo add-apt-repository ppa:webupd8team/java -y  
sudo apt-get update -y  
sudo -E apt-get purge oracle-java8-installer -y 
echo debconf shared/accepted-oracle-license-v1-1 select true | sudo debconf-set-selections
echo debconf shared/accepted-oracle-license-v1-1 seen true | sudo debconf-set-selections 
sudo -E apt-get install oracle-java8-installer -y  

#Check Java Version
sudo java -version 

# Install Java on each slaves
nodes=8
for i in `seq 1 $nodes`
do
	ssh hadoop-slave-$i '
	sudo apt-get purge openjdk-\* -y
	sudo apt-get update -y
	sudo apt-get install python-software-properties -y  
	sudo add-apt-repository ppa:webupd8team/java -y 
	sudo apt-get update -y
	sudo -E apt-get purge oracle-java8-installer -y
	echo debconf shared/accepted-oracle-license-v1-1 select true | sudo debconf-set-selections
	echo debconf shared/accepted-oracle-license-v1-1 seen true | sudo debconf-set-selections  
	sudo -E apt-get install oracle-java8-installer -y  
	sudo java -version 
	sudo cat >> ~/.bashrc <<EOF
	export HADOOP_HOME=/usr/local/hadoop  
	export HADOOP_MAPRED_HOME=/usr/local/hadoop  
	export HADOOP_COMMON_HOME=/usr/local/hadoop  
	export HADOOP_HDFS_HOME=/usr/local/hadoop  
	export YARN_HOME=/usr/local/hadoop  
	export HADOOP_COMMON_LIB_NATIVE_DIR=/usr/local/hadoop/lib/native  
	export JAVA_HOME=/usr/  
	export PATH=$PATH:/usr/local/hadoop/sbin:/usr/local/hadoop/bin:$JAVA_PATH/bin  
	'
	ssh hadoop-slave-$i '
	cd  /etc/
	sudo chmod 777 hosts
	sudo cat /home/ubuntu/ip-info >> hosts
	source ~/.bashrc'
done

#Update on Master only
cd /usr/local 
sudo rm hadoop-2.7.1.tar.* 
#Download hadoop tar
sudo wget https://archive.apache.org/dist/hadoop/core/hadoop-2.7.1/hadoop-2.7.1.tar.gz
#untar 
sudo tar xzf hadoop-2.7.1.tar.gz  
sudo rm -r hadoop
sudo mkdir hadoop  
#rename hadoop folder
sudo mv hadoop-2.7.1/* hadoop/  

#Update Hadoop Envoirnment variables
sudo cat >> ~/.bashrc <<EOF
export HADOOP_HOME=/usr/local/hadoop  
export HADOOP_MAPRED_HOME=/usr/local/hadoop  
export HADOOP_COMMON_HOME=/usr/local/hadoop  
export HADOOP_HDFS_HOME=/usr/local/hadoop  
export YARN_HOME=/usr/local/hadoop  
export HADOOP_COMMON_LIB_NATIVE_DIR=/usr/local/hadoop/lib/native  
export JAVA_HOME=/usr/  
export PATH=$PATH:/usr/local/hadoop/sbin:/usr/local/hadoop/bin:/bin  
EOF

source ~/.bashrc


cd /usr/local/hadoop/etc/hadoop

#Updating each configuration files
sudo chmod 777 hadoop-env.sh
sudo cat >> hadoop-env.sh << EOL
export JAVA_HOME=/usr/  
EOL


sudo chmod 777 hdfs-site.xml
sudo cat > hdfs-site.xml << EOL
<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
<configuration>  
<property>  
<name>dfs.replication</name>  
<value>1</value>  
</property>  
<property>  
<name>dfs.name.dir</name>  
<value>file:///mnt/raid/dfs/hadoop/hadoopinfra/hdfs/namenode</value>  
</property>  
<property>  
<name>dfs.data.dir</name>  
<value>file:///mnt/raid/dfs/hadoop/hadoopinfra/hdfs/datanode</value >  
</property>  
<property>  
<name>dfs.block.size</name> 
<value>1073741824</value >  
</property> 
</configuration>  
EOL


sudo chmod 777 core-site.xml
sudo cat > core-site.xml << EOL
<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
<configuration>  
<property>  
<name>fs.default.name</name>  
<value>hdfs://hadoop-master:9000</value>  
</property>
<property>  
<name>dfs.permissions</name>  
<value>false</value>  
</property>  
<property>
<name>hadoop.tmp.dir</name>
<value>file:///mnt/raid/tmp/mapred/local</value>
</property>
</configuration>  
EOL

sudo chmod 777 mapred-site.xml.template
sudo cp mapred-site.xml.template mapred-site.xml
sudo chmod 777 mapred-site.xml
sudo cat > mapred-site.xml << EOL
<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>
<configuration>  
<property>  
<name>mapreduce.framework.name</name>  
<value>yarn</value>  
</property>  
<property>
<name>mapreduce.job.reduces</name>
<value>8</value>
</property>
</configuration>  
EOL

sudo chmod 777 yarn-site.xml
sudo cat > yarn-site.xml << EOL
<?xml version="1.0"?>
<configuration>
<!-- Site specific YARN configuration properties -->
<property>
 <name>yarn.nodemanager.aux-services</name>
 <value>mapreduce_shuffle</value>
</property>
<property>
 <name>yarn.resourcemanager.scheduler.address</name>
 <value>hadoop-master:8030</value>
</property> 
<property>
 <name>yarn.resourcemanager.address</name>
 <value>hadoop-master:8032</value>
</property>
<property>
 <name>yarn.resourcemanager.webapp.address</name>
 <value>hadoop-master:8088</value>
</property>
<property>
 <name>yarn.resourcemanager.resource-tracker.address</name>
 <value>hadoop-master:8031</value>
</property>
<property>
 <name>yarn.resourcemanager.admin.address</name>
 <value>hadoop-master:8033</value>
</property>
</configuration> 
EOL

#Adding all the masters
cd $HADOOP_HOME/etc/hadoop/
sudo chmod 777 $HADOOP_HOME/etc/hadoop/masters
sudo cat > masters << EOL
hadoop-master
EOL
sudo chmod 777 $HADOOP_HOME/etc/hadoop/masters

#Adding all the slaves
cd $HADOOP_HOME/etc/hadoop/
sudo chmod 777 $HADOOP_HOME/etc/hadoop/slaves
sudo cat > slaves << EOL
hadoop-slave-1
hadoop-slave-2
hadoop-slave-3
hadoop-slave-4
hadoop-slave-5
hadoop-slave-6
hadoop-slave-7
hadoop-slave-8
EOL

cd /usr/local/

#sending hadoop folder to all slaves
nodes=8
for i in `seq 1 $nodes`
do
	scp -r hadoop hadoop-slave-$i:/usr/local/
done


