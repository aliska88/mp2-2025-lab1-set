// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include <stdexcept>

#include "tbitfield.h"

    TBitField::TBitField(int len) {
  if (len < 0)
    throw std::exception("Negative size");
  else
    BitLen = len;

  MemLen = (BitLen + (sizeof(TELEM) * 8) - 1) / (sizeof(TELEM) * 8);

  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++) pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) {
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++) {
    pMem[i] = bf.pMem[i];
  }
}

TBitField::~TBitField() {
  delete[] pMem;
  pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const  // индекс Мем для бита n
{
  return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const  // битовая маска для бита n
{
  return (TELEM)1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const  // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n)  // установить бит
{
  if (n < 0 || n >= BitLen) {
    throw std::out_of_range("Bit index is outside the bounds [0, BitLen - 1].");
  }

  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  pMem[index] |= mask;
}

void TBitField::ClrBit(const int n)  // очистить бит
{
  if (n < 0 || n >= BitLen) {
    throw std::out_of_range("Bit index is outside the bounds [0, BitLen - 1].");
  }

  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const  // получить значение бита
{
  if (n < 0 || n >= BitLen) {
    throw std::out_of_range("Bit index out of bounds. Stick to [0, BitLen - 1].");
  }

  return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField &TBitField::operator=(const TBitField &bf)  // присваивание
{
  if (this == &bf) {
    return *this;
  }

  delete[] pMem;
  BitLen = bf.BitLen;
  MemLen = bf.MemLen;
  pMem = new TELEM[MemLen];
  for (int i = 0; i < MemLen; i++) {
    pMem[i] = bf.pMem[i];
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const  // сравнение
{
  if (BitLen != bf.BitLen) {
    return 0;
  }

  for (int i = 0; i < MemLen; i++) {
    if (pMem[i] != bf.pMem[i]) {
      return 0;
    }
  }
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const  // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf)  // операция "или"
{
    int maxBitLen = std::max(BitLen, bf.BitLen);
    TBitField result(maxBitLen);
    int minMemLen = std::min(MemLen, bf.MemLen);
    for (int i = 0; i < minMemLen; i++) {
        result.pMem[i] = pMem[i] | bf.pMem[i];
    }
    const TBitField *longer;
    if (MemLen > bf.MemLen) {
        longer = this;
    } else if (bf.MemLen > MemLen) {
        longer = &bf;
    } else { 
        return result; 
    }
    for (int i = minMemLen; i < longer->MemLen; i++) {
        result.pMem[i] = longer->pMem[i];
    }
    return result;
}

TBitField TBitField::operator&(const TBitField &bf)  // операция "и"
{
  int minBitLen = std::min(BitLen, bf.BitLen);

  TBitField result(minBitLen);

  for (int i = 0; i < result.MemLen; i++) {
    result.pMem[i] = pMem[i] & bf.pMem[i];
  }

  return result;
}

TBitField TBitField::operator~(void)  // отрицание
{
  TBitField result(BitLen);

  for (int i = 0; i < MemLen; i++) {
    result.pMem[i] = ~pMem[i];
  }

  return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf)  // ввод
{
  delete[] bf.pMem;
  bf.pMem = nullptr;

  std::string s;
  if (!(istr >> s)) {
    return istr;
  }

  bf.BitLen = s.length();
  bf.MemLen = (bf.BitLen + (sizeof(TELEM) - 1)) / (sizeof(TELEM));
  bf.pMem = new TELEM[bf.MemLen];
  for (int i = 0; i < bf.MemLen; i++) {
    bf.pMem[i] = 0;
  }

  for (int i = 0; i < bf.BitLen; i++) {
    if (s[i] == '1') {
      bf.SetBit(i);
    }
  }

  return istr;
}

    ostream &
    operator<<(ostream &ostr, const TBitField &bf)  // вывод
{
  for (int i = 0; i < bf.BitLen; i++) {
    ostr << (bf.GetBit(i) ? '1' : '0');
  }

  return ostr;
}
