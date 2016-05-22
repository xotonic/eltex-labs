
/*
 * USAGE
 * =====
 * dmesg -w
 * insmod chardev.ko
 * mknod /dev/chardev c [major] 0
 * cat /dev/chardev
 * echo "[seed]" > /dev/chardev
 * rm -fR /dev/chardev
 * rmmod chardev.ko
 */

/*
 *  chardev.c: Создает символьное устройство, доступное только для чтения
 *  возвращает сообщение, с указанием количества произведенных попыток чтения из файла устройства
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/random.h>
#include <asm/uaccess.h>        /* определение функции put_user */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dmitry Kuzmin");
MODULE_DESCRIPTION("Char device");
/*
 *  Прототипы функций, обычно их выносят в заголовочный файл (.h)
 */
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "chardev"   /* Имя устройства, будет отображаться в /proc/devices   */
#define BUF_LEN 80                      /* Максимальная длина сообщения */
#define PREFIX DEVICE_NAME": "
/*
 * Глобальные переменные, объявлены как static, воизбежание конфликтов имен.
 */

static int Major;             /* Старший номер устройства нашего драйвера */
static int Device_Open = 0;   /* Устройство открыто?
                               * используется для предотвращения одновременного
                               * обращения из нескольких процессов */
static char Message[BUF_LEN];     /* Здесь будет собираться текст сообщения */
static char msg[BUF_LEN];     /* Здесь будет собираться текст сообщения */
static char *msg_Ptr;
static int rand_min = 0, rand_max = 1000;

static struct file_operations fops = {
  .read = device_read,
  .write = device_write,
  .open = device_open,
  .release = device_release
};

/*
 * Функции
 */


/*
        Находит среднее по масссиву a размерности n
*/
int avg(int *a, int n) {
  int sum = 0;
  int i;
  int avg;
  for (i = 0; i < n; i++) {
    sum += a[i];
  }
  avg = sum / n;
  return avg;
}

/*
 *  Генерирует массив и вычисляет его среднее
 */
static int mem[BUF_LEN];

int work(int randmin, int randmax)
{
  int n = BUF_LEN;//rand()%1000;
  int i, average;
  int random;
  //srand(sseed);
  //float* mem = (float*)malloc(sizeof(float)* n);
  for (i = 0; i < n; i++)
  {
      random = get_random_int();
      if (random < 0) random = - random;
      mem[i] = randmin + random%(randmax - randmin);
  }
  average = avg(mem, n);
  //printf("Child: pid = %d\tavg = %f\n", (int)getpid(), average);
  //free(mem);
  return average;
}

int init_module(void)
{
  Major = register_chrdev(0, DEVICE_NAME, &fops);

  if (Major < 0) {
    printk(PREFIX"Registering the character device failed with %d\n",
            Major);
    return Major;
  }

  printk(PREFIX"Initialising module. Major number = %d.\n", Major);

  return 0;
}

void cleanup_module(void)
{
  /*
   * Отключение устройства
   */
    printk(PREFIX"Cleaning up module\n");
    unregister_chrdev(Major, DEVICE_NAME);
}

/*
 * Обработчики
 */

/*
 * Вызывается, когда процесс пытается открыть файл устройства, например командой
 * "cat /dev/chardev"
 */
static int device_open(struct inode *inode, struct file *file)
{
  //static int counter = 0;
  printk(PREFIX"Device file opened\n");
  if (Device_Open)
    return -EBUSY;
  Device_Open++;
  //sprintf(msg, "OPEN %d\n", counter++);
  printk(PREFIX"Random min = %d, max = %d\n", rand_min, rand_max);
  sprintf(msg, "%d\n", work(rand_min, rand_max));
  msg_Ptr = msg;
  try_module_get(THIS_MODULE);

  return SUCCESS;
}

/*
 * Вызывается, когда процесс закрывает файл устройства.
 */
static int device_release(struct inode *inode, struct file *file)
{

  printk(PREFIX"Device file closed\n");
  Device_Open--;                /* Теперь мы готовы обслужить другой процесс */

  /*
   * Уменьшить счетчик обращений, иначе, после первой же удачной попытки открыть файл устройства,
   * вы никогда не сможете выгрузить модуль.
   */
  module_put(THIS_MODULE);

  return 0;
}

/*
 * Вызывается, когда процесс пытается прочитать уже открытый файл устройства
 */
static ssize_t device_read(struct file *filp, /* см. include/linux/fs.h   */
         char *buffer,                        /* буфер, куда надо положить данные */
         size_t length,                       /* размер буфера */
         loff_t * offset)
{

  /*
   * Количество байт, фактически записанных в буфер
   */
  int bytes_read = 0;

  printk(PREFIX"Reading from device file\n");
  /*
   * Если достигли конца сообщения,
   * вернуть 0, как признак конца файла
   */
  if (*msg_Ptr == 0)
    return 0;

  /*
   * Перемещение данных в буфер
   */
  while (length && *msg_Ptr) {

    /*
     * Буфер находится в пространстве пользователя (в сегменте данных),
     * а не в пространстве ядра, поэтому простое присваивание здесь недопустимо.
     * Для того, чтобы скопировать данные, мы используем функцию put_user,
     * которая перенесет данные из пространства ядра в пространство пользователя.
     */
    put_user(*(msg_Ptr++), buffer++);

    length--;
    bytes_read++;
  }

  /*
   * В большинстве своем, функции чтения возвращают количество байт, записанных в буфер.
   */
  return bytes_read;
}

/*
 * например так: echo "hi" > /dev/chardev
 * Вызывается, когда процесс пытается записать в устройство,
 */
static ssize_t
device_write(struct file *filp, const char *buff, size_t len, loff_t * off)
{
  int parsed = 0, i;
  printk(PREFIX"Writing to device file\n");
  for (i = 0; i < len && i < BUF_LEN; i++)
   get_user(Message[i], buff + i);

 //Message_Ptr = Message;
  parsed = sscanf(Message, "%d %d", &rand_min, &rand_max);
  if (parsed != 2) return -EINVAL;
 /*
  * Вернуть количество принятых байт
  */
 return i;
  //return -ENOSYS;
}
