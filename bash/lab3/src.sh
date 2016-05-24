#!/bin/bash

print_alarms()
{
    echo "= CURRENT ALARMS ="
    id=1
    while read line
    do
        read m h z <<< "$line"
        echo "$id) $h : $m"
        ((id++))
    done <alarms.txt

    if [ $id -eq 1 ]
    then
      echo "NO ALARMS"
    fi
    echo "=================="
}

print_alarms
echo "1 - New alarm"
echo "2 - Delete alarm"
echo "3 - Print alarms"
echo "9 - Save and exit"
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
    9)
    echo "SAVE AND EXIT"
    crontab alarms.txt
    ans=0
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
    printf "ID>>"
    read id
    sed -i "$id d" alarms.txt
    echo "OK"
    ;;
    3)
    print_alarms
    ;;
    *)
    echo "UNKNOWN"
    ;;
  esac
done
echo "=================="

#echo "$ans";
