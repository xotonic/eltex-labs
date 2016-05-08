#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

// для отладки
#define C(x)                                                                   \
  if (x < 0) {                                                                \
    printf("Fail in %s %d, error = %d\n", __FUNCTION__, __LINE__, (int)x);                         \
    exit(EXIT_FAILURE);                                                        \
  }

// Данные мыши
static  int VENDOR_ID = 0;
static  int PRODUCT_ID = 0;
static  int INTERRUPT_ENDPOINT = 0x81;
static const int INTERFACE_NUMBER = 0;
static const int TIMEOUT_MS = 5000;
// Опытным путем выяснено, что для минимального
// функционала (кнопки, перемещение, колесико)
// нам достаточно знать 6 первых байтов пакета
// При этом координаты упакованы в 3 байта (да-да, в 12-битные байты)
static const int TRANSFER_SIZE = 6;

/* СОБЫТИЯ МЫШИ */
static const char M_LEFT_BUTTON    = 0x1;
static const char M_RIGHT_BUTTON   = 0x2;
static const char M_MIDDLE_BUTTON  = 0x4;

// для отладки
void printBits(size_t const size, void const * const ptr)
{
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;

  for (i = size - 1; i >= 0; i--)
  {
    for (j = 7; j >= 0; j--)
    {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
    printf(" ");
  }
  puts("");
}

int main(int argc, char** argv)
{

  // Парсинг параметров командной строки
  int option;
  while ((option = getopt(argc, argv, "v:p:e:")) != -1) {
    switch (option) {
    case 'p' : PRODUCT_ID = (int)strtol(optarg, NULL, 0);
      break;
    case 'v' : VENDOR_ID = (int)strtol(optarg, NULL, 0);
      break;
    case 'e' : INTERRUPT_ENDPOINT = (int)strtol(optarg, NULL, 0);
      break;
    default: printf("Usage:\n"\
                      " -v 0x<vendor_id> \n"\
                      " -p 0x<product_id>\n"\
                      " [-e] 0x<endpoint>\n");
      exit(EXIT_FAILURE);
    }
  }

  // Инициализация
  libusb_context *context = NULL;
  libusb_device_handle * dev = NULL;
  // Данные пакета
  char data[TRANSFER_SIZE];
  // Число полученных битов от прерывания
  int transferred = 0;

  C(libusb_init(&context));
  // Показывать ворнинги и ошибки
  libusb_set_debug(context, 3);

  // Сразу берем хандлер устройства без получения всего списка
  dev = libusb_open_device_with_vid_pid(context,
                                        VENDOR_ID, PRODUCT_ID);
  if (dev == NULL)
  {
    printf("Cant open device\n");
    libusb_exit(0);
    return EXIT_FAILURE;
  }

  // Включаем автоматическое выключение драйвера ядра
  // и включение при возникновении ошибки
  // Если делать это вручную, то при фэйле
  // драйвер не будет включен и устройство будет потеряно
  libusb_set_auto_detach_kernel_driver(dev, 1);

  //C(libusb_detach_kernel_driver(dev, INTERFACE_NUMBER));

  // Именно здесь драйвер и отключится
  C(libusb_claim_interface(dev, INTERFACE_NUMBER));

  int result = 0;

  // -7 - это LIBUSB_ERROR_TIMEOUT,
  // т.е. завершаем работу по его истечении
  while (result != -7)
  {
    // Возвожно я что-то не учел, но
    // функция всегда возвращает код ошибки
    // LIBUSB_ERROR_OVERFLOW (-8).
    // Однако данные (data) при этом записываются
    result = libusb_interrupt_transfer(
               dev,
               INTERRUPT_ENDPOINT,
               data,
               TRANSFER_SIZE,
               &transferred,
               TIMEOUT_MS);
    // Если все "успешно"
    if (result == -8)
      // Если мы получили хоть сколько-то байтов
      if (transferred > 0 )
      {
        // Пакет в битах
        //printBits(TRANSFER_SIZE, data);

        // 12-битные байты координат (дельт)
        int x_byte = (*(data + 2));
        int y_byte = (*(data + 3)) >> 4;
        // Байт кнопок
        char button_byte = *(data + 1);
        // Байт колесика
        char wheel_byte = *(data + 5);

        // Вывод
        printf("x %d\ty %d\t", (int)x_byte, (int)y_byte);

        if (button_byte & M_LEFT_BUTTON)
          printf("LEFT ");
        if (button_byte & M_MIDDLE_BUTTON)
          printf("MIDDLE ");
        if (button_byte & M_RIGHT_BUTTON)
          printf("RIGHT ");

        if (wheel_byte > 0)
          printf("WHEEL-DOWN");
        else if (wheel_byte < 0)
          printf("WHEEL-UP");
        printf("\n");

      }
      else
      {
        printf("No data transferred\n");
      }
    else
    {
      // Если таймаут, то выходим на след итерации
      if (result == -7)
        printf("Timeout\n");
      else
        printf("Error: %d\n", result);
    }
  }

  // Убираем за собой
  C(libusb_release_interface(dev, INTERFACE_NUMBER));
  libusb_close(dev);
  libusb_exit(context);

  return EXIT_SUCCESS;
}