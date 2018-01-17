
# purge openjdk if installed to remove conflict  
sudo apt-get purge openjdk-\* -y
sudo apt-get update -y
sudo apt-get install python-software-properties -y
sudo add-apt-repository ppa:webupd8team/java -y
sudo apt-get update -y
sudo -E apt-get purge oracle-java8-installer -y
sudo -E apt-get install oracle-java8-installer -y
#check java version
sudo java -version

#generate rsa key
ssh-keygen -t rsa -P ""

#add key in authorized_keys
cat $HOME/.ssh/id_rsa.pub >> $HOME/.ssh/authorized_keys

#download and install hadoop 
cd /usr/local
sudo rm hadoop-2.7.1.tar.*
sudo wget https://archive.apache.org/dist/hadoop/core/hadoop-2.7.1/hadoop-2.7.1.tar.gz
sudo tar xzf hadoop-2.7.1.tar.gz
sudo rm -r hadoop
sudo mkdir hadoop
#rename hadoop
sudo mv hadoop-2.7.1/* hadoop/


sudo cat >> ~/.bashrc <<EOF
export HADOOP_HOME=/usr/local/hadoop  
export HADOOP_MAPRED_HOME=/usr/local/hadoop  
export HADOOP_COMMON_HOME=/usr/local/hadoop  
export HADOOP_HDFS_HOME=/usr/local/hadoop  
export YARN_HOME=/usr/local/hadoop  
export HADOOP_COMMON_LIB_NATIVE_DIR=/usr/local/hadoop/lib/native  
export JAVA_HOME=/usr/  
export PATH=$PATH:/usr/local/hadoop/sbin:/usr/local/hadoop/bin:/usr/bin:/bin:/opt/spark/bin  
EOF

#sudo cat ~/.bashrc

source ~/.bashrc

cd /usr/local/hadoop/etc/hadoop

#update all configuration files
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
<value>file:///mnt/raid/dfs/hadoop/hadoopinfra/hdfs/namenode </value>  
</property>  
<property>  
<name>dfs.data.dir</name>  
<value>file:///mnt/raid/dfs/hadoop/hadoopinfra/hdfs/datanode </value >  
</property>  
<property>  
<name>dfs.block.size</name> 
<value>536870912</value >  
</property> 
</configuration>  
EOL


sudo chmod 777 mapred-site.xml
sudo chmod 777 mapred-site.xml.template
sudo cp mapred-site.xml.template mapred-site.xml
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
<value>1</value>
</property>
</configuration>  
EOL

sudo chmod 777 yarn-site.xml
sudo cat > yarn-site.xml << EOL
<?xml version="1.0"?>
<configuration>  
<property>  
<name>yarn.nodemanager.aux-services</name>  
<value>mapreduce_shuffle</value>  
</property>  
<property>  
<name>yarn.nodemanager.disk-health-checker.max-disk-utilization-per-disk-percentage</name>  
<value>98.5</value>  
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
<value>hdfs://localhost:9000</value>  
</property>  
<property>
<name>hadoop.tmp.dir</name>
<value>file:///mnt/raid/tmp/mapred/local</value>
</property>
</configuration>  
EOL


sudo chmod 777 /usr/local/*
cd /usr/local/
cd ~
                                                                                                                 