#include <Arduino.h>
#include <stdlib.h>

template <uint16_t N>
class TORICA_MoveMedian
{
public:
  TORICA_MoveMedian(float value);

  float add(float value);
  float get();

private:
  float _buffer[N];          // index大 = value大
  uint16_t _buffer_index[N]; // new=0, old=N-1
  float _median = 0;
};

template <uint16_t N>
TORICA_MoveMedian<N>::TORICA_MoveMedian(float value)
{
  for (uint16_t i = 0; i < N; i++)
  {
    _buffer[i] = value;
    _buffer_index[i] = i;
  }
  _median = value;
}

template <uint16_t N>
float TORICA_MoveMedian<N>::get()
{
  return _median;
}

template <uint16_t N>
float TORICA_MoveMedian<N>::add(float value)
{
  // 一番古い値のindexを取得する
  uint16_t oldest_index;
  for (oldest_index = 0; oldest_index < N; oldest_index++)
  {
    if (_buffer_index[oldest_index] == N - 1)
    {
      break;
    }
  }

  // 一番古い値を削除し，index0側に値を寄せる
  // ToDo: 遅かったら memmove
  for (uint16_t i = oldest_index; i < N - 1; i++)
  {
    _buffer[i] = _buffer[i + 1];
    _buffer_index[i] = _buffer_index[i + 1];
  }

  // indexを更新する
  for (uint16_t i = 0; i < N - 1; i++)
  {
    _buffer_index[i]++;
  }

  // 値の大小を比較しながら走査し，新しい値の場所を探す
  // ToDo: 二分探索
  uint16_t new_index;
  if (value < _buffer[0])
  {
    new_index = 0;
  }
  else
  {
    for (new_index = 0; new_index < N - 2; new_index++)
    {
      if (value > _buffer[new_index + 1])
      {
        break;
      }
    }
  }

  // 古い値をindexN側に一つずつずらす
  // ToDo: 遅かったら memmove
  for (uint16_t i = N - 1; i > new_index; i--)
  {
    _buffer[i] = _buffer[i - 1];
    _buffer_index[i] = _buffer_index[i - 1];
  }

  // 新しい値を挿入する
  _buffer[new_index] = value;
  _buffer_index[new_index] = 0;

  // 中央値を計算する
  if (N % 2 == 0)
  {
    _median = (_buffer[N / 2 - 1] + _buffer[N / 2]) / 2;
  }
  else
  {
    _median = _buffer[N / 2];
  }

  return _median;
}
