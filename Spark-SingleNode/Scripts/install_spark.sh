#Download spark
wget https://d3kbcqa49mib13.cloudfront.net/spark-2.2.0-bin-hadoop2.7.tgz
#untar spark
sudo tar zxvf spark-2.2.0-bin-hadoop2.7.tgz -C /opt
sudo ln -fs spark-2.2.0-bin-hadoop2.7 /opt/spark
#set environment variable to bashrc file
sudo cat >> ~/.bashrc <<EOF
export SPARK_HOME=/opt/spark
EOF

source ~/.bashrc
#check whether spark is installed or not
spark-submit --version
#updating spark configuration
cp /opt/spark/conf/spark-defaults.conf.template /opt/spark/conf/spark-defaults.conf

sudo cat >> /opt/spark/conf/spark-defaults.conf <<EOF

spark.local.dir	/mnt/raid/spark_temp
EOF