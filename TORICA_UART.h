#include <Arduino.h>
class TORICA_UART {
  public:
    TORICA_UART(Stream *_serial) {
      serial = _serial;
    }

    int readUART();
    float UART_data[32];

  private:
    int i_buff = 0;
    char buff[256];
    char *p;
    Stream *serial;
};
