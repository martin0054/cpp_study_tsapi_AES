#!/bin/sh
source /etc/profile
SERVICE_NAME=ALCOM_CTI
SERVICE_PATH=/ALCOM_CTI

echo "#################################################"
echo " Starting $SERVICE_NAME Service .........."
Cnt=`ps -ef | grep $SERVICE_NAME | grep -v grep | grep -v sh | wc -l`
PROCESS=`ps -ef | grep $SERVICE_NAME | grep -v grep | grep -v sh | awk '{print $1}'`

if [ $Cnt -ne 0 ] 
then
   echo " $SERVICE_NAME Service is already running."
else
   cd $SERVICE_PATH
   nohup $SERVICE_PATH/$SERVICE_NAME 1> /dev/null 2>&1 &
   echo " $SERVICE_NAME Service Start Complete."
fi

echo "#################################################"
