#!/bin/sh
source /etc/profile
SERVICE_NAME=ALCOM_CTI

echo "#################################################"
Cnt=`ps -ef | grep $SERVICE_NAME | grep -v grep | grep -v sh | wc -l`

if [ $Cnt -ne 0 ];
then
   echo " $SERVICE_NAME Service is running(Active)"
else
   echo " $SERVICE_NAME Service stopped."
fi

echo "#################################################"
