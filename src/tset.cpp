// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include "tbitfield.h"

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField() { 
  return BitField; 
}

int TSet::GetMaxPower(void) const  // получить макс. к-во эл-тов
{
  return MaxPower;
}

int TSet::IsMember(const int Elem) const  // элемент множества?
{
  return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)  // включение элемента множества
{
  BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)  // исключение элемента множества
{
  BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet &TSet::operator=(const TSet &s)  // присваивание
{
  if (this == &s) {
    return *this;
  }
  MaxPower = s.MaxPower;
  BitField = s.BitField;
  return *this;
}

int TSet::operator==(const TSet &s) const  // сравнение
{
  return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet &s) const  // сравнение
{
  return !(*this == s);
}

TSet TSet::operator+(const TSet &s)  // объединение
{
  if (MaxPower != s.MaxPower) {
    return TSet(0);
  }

  return TSet(BitField | s.BitField);
}

TSet TSet::operator+(const int Elem)  // объединение с элементом
{
  TSet Res = *this;
  Res.InsElem(Elem);
  return Res;
}

TSet TSet::operator-(const int Elem)  // разность с элементом
{
  TSet Res = *this;
  Res.DelElem(Elem);
  return Res;
}

TSet TSet::operator*(const TSet &s)  // пересечение
{
  if (MaxPower != s.MaxPower) {
    return TSet(0);
  }

  return TSet(BitField & s.BitField);
}

TSet TSet::operator~(void)  // дополнение
{
  return TSet(~BitField);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s)  // ввод
{
  istr >> s.BitField;
  return istr;
}

ostream &operator<<(ostream &ostr, const TSet &s)  // вывод
{
  ostr << s.BitField;
  return ostr;
}
