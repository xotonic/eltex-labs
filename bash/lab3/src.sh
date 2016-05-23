#!/bin/bash
echo "= CURRENT ALARMS ="

while read line
do
  #echo -e "$line"
  #echo ?????????
done <alarms.txt

echo "=================="
echo "1 - New alarm"
echo "2 - Delete alarm"
echo "0 - Exit"


ans=-1
hour=-1
min=-1
while [ $ans -ne 0 ]
do
  printf ">>"
  read ans
  case $ans in
    0)
    echo "EXIT"
    ;;
    1)
    echo "CREATE"
    printf "HOURS>>"
    read hour
    printf "MINUTES>>"
    read min
    str="$min $hour * * * /usr/bin/mpg123 -q ~/Documents/bash/lab3/meow.mp3"
    echo "$str" >> alarms.txt
    echo "OK"
    ;;
    2)
    echo "DELETE"
    ;;
    *)
    echo "UNKNOWN"
    ;;
  esac
done
echo "=================="

#echo "$ans";
