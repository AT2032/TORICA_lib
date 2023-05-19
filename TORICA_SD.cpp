#include "TORICA_SD.h"
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

bool TORICA_SD::begin()
{
  SERIAL_USB.print("Initializing SD card...");
#if defined(SEEED_XIAO_RP2040)
  SPI.setRX(SD_SPI_MISO);
  SPI.setCS(SD_SPI_CSn);
  SPI.setSCK(SD_SPI_SCK);
  SPI.setTX(SD_SPI_MOSI);
  if (!SD.begin(cs_SD, SPI))
#else
  if (!SD.begin(cs_SD))
#endif
  {
    SERIAL_USB.println("Card failed, or not present");
    SDisActive = false;
    return false;
  }
  new_file();
  SERIAL_USB.println("card initialized.");
  SDisActive = true;
  return true;
}

void TORICA_SD::add_str(char str[])
{
  if (SDisActive)
  {
    int str_len = strlen(str);
    memcpy((void *)&SD_buf[SD_buf_index][SD_buf_count[SD_buf_index]], (void *)str, str_len * sizeof(char));
    if (SD_buf_count[SD_buf_index] < TORICA_SD_BUF_SIZE - str_len)
    {
      SD_buf_count[SD_buf_index] += str_len;
    }
    else
    {
      SERIAL_USB.println("overflow");
    }
  }
}

void TORICA_SD::new_file()
{
  String s;
  int fileNum = 0;
  while (1)
  {
    s = "LOG";
    if (fileNum < 10)
    {
      s += "000";
    }
    else if (fileNum < 100)
    {
      s += "00";
    }
    else if (fileNum < 1000)
    {
      s += "0";
    }
    s += fileNum;
    s += ".CSV";
    s.toCharArray(fileName, 16);
    if (!SD.exists(fileName))
      break;
    fileNum++;
  }
  file_time = millis();
  file_size = 0;
}

void TORICA_SD::flash()
{
  uint32_t SD_time = millis();
  int previous_index = SD_buf_index;
  SD_buf_index = (SD_buf_index + 1) % 2;

  if (millis() - file_time > 10 * 60 * 1000)
  {
    new_file();
  }
  if (file_size >= TORICA_SD_MAX_FILE_SIZE)
  {
    SERIAL_USB.println("TORICA_SD_MAX_FILE_SIZE");
    new_file();
  }

  dataFile = SD.open(fileName, FILE_WRITE);
  if (dataFile)
  {
    dataFile.write((char *)SD_buf[previous_index], sizeof(char) * (SD_buf_count[previous_index]));
    file_size += sizeof(char) * (SD_buf_count[previous_index]);

    dataFile.close();

    SERIAL_USB.println("SD_buf_count,SD_total");
    SERIAL_USB.print(SD_buf_count[previous_index]);
    SERIAL_USB.print(",");
    uint32_t SD_total = millis() - SD_time;
    SERIAL_USB.println(SD_total);

    if (SD_total > 2000)
    {
      SERIAL_USB.println("too long time");
      end();
    }
  }
  else
  {
    SERIAL_USB.println("error opening file");
    end();
    SDisActive = begin();
  }
  SD_buf_count[previous_index] = 0;
}

void TORICA_SD::end()
{
  SD_buf_count[0] = 0;
  SD_buf_count[1] = 0;
  SDisActive = false;
  SD.end();
}
