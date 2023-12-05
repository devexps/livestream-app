#!/bin/sh

ulimit -c unlimited

dir_exe=../cron

# Enter ../cron
cd $dir_exe
if [ $? -ne 0 ]; then
    echo "cant change to $dir_exe"
    exit 1
fi

./check_submcd_log.sh

./check_mcd.sh

./check_ccd.sh

exit 0