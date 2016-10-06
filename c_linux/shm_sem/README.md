
СЕМАФОРЫ И ОБЩАЯ ПАМЯТЬ
=======================

Родительский процесс создает семафор, общую память и заполняет ее.
Дочерние процессы получают на вход ID семафора и памяти,
читают и изменяют ее. Делать это одновременно может только 1 процесс,
остальные встают в очередь.
Затем по нажатию <kbd>ENTER</kbd> родитель освободает память и удаляет семафор

USAGE
-----
```
$ ./sem
Creating new semaphore: max 1 processes
Generating 1000000 random numbers...
Starting 10 processes...
Press ENTER to free memory
pid 7929: Locked - 0
pid 7929: Unlocked - 0, avg = 499.547943
pid 7930: Locked - 0
pid 7932: Locked - 0
pid 7930: Unlocked - 0, avg = 999.095886
pid 7933: Locked - 0
pid 7932: Unlocked - 0, avg = 1998.191772
pid 7933: Unlocked - 0, avg = 3996.383545
pid 7931: Locked - 0
pid 7931: Unlocked - 0, avg = 7992.767090
pid 7934: Locked - 0
pid 7935: Locked - 0
pid 7936: Locked - 0
pid 7935: Unlocked - 0, avg = 31971.068359
pid 7934: Unlocked - 0, avg = 15985.534180
pid 7937: Locked - 0
pid 7937: Unlocked - 0, avg = 127884.273438
pid 7938: Locked - 0
pid 7936: Unlocked - 0, avg = 63942.136719
pid 7938: Unlocked - 1, avg = 255768.546875
$ <ENTER>
Removing shared memory and semaphore...
Done
```