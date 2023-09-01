#!/bin/sh
source /etc/profile
SERVICE_NAME=ALCOM_CTI

echo "#################################################"
echo " Shutting Down $SERVICE_NAME Service .........."
Cnt=`ps -ef | grep $SERVICE_NAME | grep -v grep | grep -v sh | wc -l`
PROCESS=`ps -ef | grep $SERVICE_NAME | grep -v grep | grep -v sh | awk '{print $2}'`

echo " Cnt : $Cnt"
echo " PROCESS : $PROCESS"
if [ $Cnt -ne 0 ];
then
   kill -9 $PROCESS
   sleep 1
   
   PROCESS_COUNT=`ps -ef | grep $SERVICE_NAME | grep -v grep | grep -v sh | wc -l`
   if [ $PROCESS_COUNT -ne 0 ];
   then
       echo " $SERVICE_NAME Service shutting down fail."
   else
       echo " $SERVICE_NAME Service stopped."
   fi    
else
   echo " $SERVICE_NAME Service stopped."
fi

echo "#################################################"
