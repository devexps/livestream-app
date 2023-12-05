#!/bin/sh

ulimit -c unlimited

name=ccd
dir_cron=`pwd`
dir_svr=../..
dir_exe=${dir_svr}/bin

cd ${dir_svr}
process=`pwd | xargs -i basename {}`"_$name"

cd ${dir_cron}
count=`ps -ef|grep "./${process} ../etc/${process}.conf"|grep -v grep|wc -l`

if [ $count -lt 1 ]; then
    echo "[`date +'%Y-%m-%d %T'`] process ${process} number:$count, fork it!"
    cd ${dir_exe};./${process} ../etc/${process}.conf
else
    echo "[`date +'%Y-%m-%d %T'`] process ${process} number is normal:$count"
fi
