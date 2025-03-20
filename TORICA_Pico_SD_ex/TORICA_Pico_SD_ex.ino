#include <TORICA_Pico_SD.h>

int cs_SD = 17; // 接続しているGPIO番号で指定
TORICA_Pico_SD sd(cs_SD);

char SD_BUF[256];

void setup()
{
  sd.begin();
  pinMode(25,OUTPUT);
}

void loop()
{
  digitalWrite(25,!digitalRead(25));
  for (int i = 0; i < 100; i++)
  {
    sprintf(SD_BUF, "TEST,%d,%.2f,%.2f\n", millis(), 12.34, 334);
    // my_torica_sd内のバッファに保存
    sd.add_str(SD_BUF);
  }
  // SDに書き込み
  sd.flash();
  delay(1000);
}