#include <Arduino.h>

/*
クラス型の定義法
class クラスの名前{
  public:     ←アクセス指定子
    メンバ関数や
    メンバ変数を入れたりする
}


*/


class TORICA_talk {
  public ://コンストラクト化
    void talk_str (char str[]);//関数のプロトタイプ宣言

    void talk_num(float num);

    void callout_altitude(float alt);

    void callout_airspeed(float spd);
};