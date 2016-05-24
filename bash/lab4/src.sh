#!/bin/bash
clgreen="\e[32m"
clclear="\e[0m"
task="*/$2 * * * * export DISPLAY=:0 && pgrep $1 || $1" #"*/$2 * * * * pgrep $1 || $1 &"

crontab -l > tmp
sed -i "/$1/d" tmp
if [ "$3" != "remove" ]; then
echo "$task" >> tmp
echo -e "I will check $clgreen$1$clclear for running every $clgreen$2$clclear minutes"
else
echo -e "I will forget about $1"
fi
crontab tmp
