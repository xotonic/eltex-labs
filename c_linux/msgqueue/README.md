ОПИСАНИЕ
========

`sent` - добавляет одну запись в очередь
`receive` - забирает все команды из очереди, выводит их и завершает работу
`receive_and_wait` - забирает команды из очереди и ждет новые

ПРИМЕР ИСПОЛЬЗОВАНИЯ
--------------------

```
$ for ((n=0;n<10;n++)); do ./send; done
Message sent
Message sent
Message sent
Message sent
Message sent
Message sent
Message sent
Message sent
Message sent
Message sent

$ ipcs -q
------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x000005d0 0          osboxes    666        270          10     

$ ./receive
pid = 8402	avg = 494.587128
pid = 8403	avg = 511.922729
pid = 8404	avg = 493.148895
pid = 8405	avg = 487.159180
pid = 8406	avg = 509.512939
pid = 8407	avg = 490.863251
pid = 8408	avg = 493.263702
pid = 8409	avg = 479.174255
pid = 8410	avg = 527.565247
pid = 8411	avg = 495.450714

$ ipcs -q

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x000005d0 0          osboxes    666        0            0   
```