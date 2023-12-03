#!/bin/sh

ulimit -c unlimited
TTC_BIN=ttcd

if [ "$1" = "stop" ] ; then
    killall -2 $TTC_BIN
elif [ "$1" = "restart" ]; then
    killall -2 $TTC_BIN
    ./$TTC_BIN
elif [ "$1" = "start" ]; then
    ./$TTC_BIN
else
    echo "usage: ttcd.sh start|stop|restart"
fi

