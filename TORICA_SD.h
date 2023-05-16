// https://qiita.com/MergeCells/items/85a860150735730faabc
#if defined(ARDUINO_SAM_DUE)
// DUE RAM 96KB
#define TORICA_SD_BUF_SIZE 32768
#define SERIAL_USB SerialUSB
#elif defined(ARDUINO_SEEED_XIAO_M0)
// XIAO RAM 32KB
#define TORICA_SD_BUF_SIZE 4096
#define SERIAL_USB Serial
#else
// other board
#define TORICA_SD_BUF_SIZE 1024
#define SERIAL_USB Serial
#endif

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

class TORICA_SD
{
public:
  TORICA_SD(int _cs_SD)
  {
    cs_SD = _cs_SD;
  }

  bool begin();
  void add_str(char str[]);
  void flash();
  bool SDisActive = false;

private:
  void new_file();
  void end();
  uint32_t file_time = 0;

  int cs_SD = LED_BUILTIN;
  char fileName[16];
  File dataFile;
  char SD_buf[2][TORICA_SD_BUF_SIZE];
  int SD_buf_index = 0;
  int SD_buf_count[2] = {0, 0};
};
