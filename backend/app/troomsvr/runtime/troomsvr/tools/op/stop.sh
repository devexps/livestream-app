#!/bin/sh

echo "Stopping service..."

cd ../../
process=`pwd | xargs -i basename {}`

CCD_PIDS=`ps -ef | grep "./${process}_ccd ../etc/${process}_ccd.conf" | grep -v grep | awk '{print $2}'`
MCD_PIDS=`ps -ef | grep "./${process}_mcd ../etc/${process}_mcd.conf" | grep -v grep | awk '{print $2}'`
LOG_SUBMCD_PIDS=`ps -ef | grep "./${process}_submcd_log ../etc/${process}_submcd_log.conf" | grep -v grep | awk '{print $2}'`
for MAIN_PID in $CCD_PIDS $MCD_PIDS $LOG_SUBMCD_PIDS
do
	kill -s USR2 $MAIN_PID
done

if [ $? != "0" ]
then
echo "Can't stop."
exit 1
else
echo "Service stopped."
exit 0
fi
