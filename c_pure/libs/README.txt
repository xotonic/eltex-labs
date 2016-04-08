# 4.2. Оформить статическую библиотеку функций и написать программу, ее использующую.

osboxes@osboxes:~/Documents/c_pure/libs$ ./main_static
Static lib example
Insert A and B floats
40 40
sum	80.000000
sub	0.000000
mult	1600.000000


# 4.3. Переоформить библиотеку, как динамическую, но подгружать статически, при компиляции.

# указываем системе путь до либы
LD_LIBRARY_PATH=~/Documents/c_pure/libs
export LD_LIBRARY_PATH
./main_dynamic_linked 

Static lib example
Insert A and B floats
40 20
sum	60.000000
sub	20.000000
mult	800.000000

# 4.4. Изменить программу для динамической загрузки функций из библиотеки

# проверяем символы

osboxes@osboxes:~/Documents/c_pure/libs$ nm -g libmylib.so 
0000000000201030 B __bss_start
                 w __cxa_finalize@@GLIBC_2.2.5
0000000000201030 D _edata
0000000000201038 B _end
0000000000000720 T _fini
                 w __gmon_start__
0000000000000580 T _init
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
                 w _Jv_RegisterClasses
0000000000000704 T mult <-----------------------!
00000000000006ea T sub	<-----------------------! наши функции
00000000000006d0 T sum	<-----------------------!

osboxes@osboxes:~/Documents/c_pure/libs$ ./main_runtime 
Runtime linking example
Insert A and B floats
40 40
sum	80.000000
sub	0.000000
mult	1600.000000

