
СИМВОЛЬНОЕ УСТРОЙСТВО
=====================

Данный модуль - псевдо-драйвер, реализующий чтение и запись данных из файла устр
ойства (/dev/chardev).При записи в файл драйвер ожидает 2 целых числа: минимальн
ая и максимальная границы для генератора случайных чисел. При чтении из файла др
айвер выводит одно целое число - среднее по массиву случайно сгенерированных чис
ел в диапазоне, заданном при записи (если запись не была произведена ни разу, из
начальный диапазон: от 0 до 1000). Также драйвер выводит лог в dmesg о вызываемы
х колбэках.

По сравнению с прошлыми вычислениями средних по массиву, текущие используют толь
ко целочисленные значения, т.к. в kernel space по умолчанию не включены операции
для работы с плавающей точкой и добавить функционал довольно сложно и вообще не
рекомендуется.

Машина, на которой проводилось тестирование и сборка:
Linux osboxes 4.2.0-16-generic #19-Ubuntu SMP Thu Oct 8 15:35:06 UTC 2015 x86_64
x86_64 x86_64 GNU/Linux

ПРОЦЕСС СБОРКИ И ПРИМЕР ЗАПУСКА
-------------------------------
# Под рутом
dmesg -w

# После чего в новом терминале
make
insmod chardev.ko
# создание файла устройства
# major - старший номер уст-ва, виден в dmesg после инициализации модуля
mknod /dev/chardev c [major] 0
cat /dev/chardev
echo "-1000 1000" > /dev/chardev
cat /dev/chardev

# удаление файла устройства и выгрузка модуля
rm -fR /dev/chardev
rmmod chardev.ko


Пример вывода dmesg:
[32451.434727] chardev: Initialising module. Major number = 247.
[32481.683815] chardev: Device file opened
[32481.683819] chardev: Random min = 0, max = 1000
[32481.683838] chardev: Reading from device file
[32481.683915] chardev: Reading from device file
[32481.683928] chardev: Device file closed
[32492.493309] chardev: Device file opened
[32492.493312] chardev: Random min = 0, max = 1000
[32492.493332] chardev: Writing to device file
[32492.493336] chardev: Device file closed
[32507.261250] chardev: Device file opened
[32507.261253] chardev: Random min = -1000, max = 1000
[32507.261269] chardev: Reading from device file
[32507.261310] chardev: Reading from device file
[32507.261320] chardev: Device file closed
[32567.778482] chardev: Cleaning up module
