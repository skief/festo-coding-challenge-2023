#pragma once
#include <cinttypes>

class Fraction{
public:
    int64_t num;
    int64_t denom;

    void simplify();

    Fraction();
    Fraction(int64_t num, int64_t denom);
};

Fraction operator+(const Fraction& l, const Fraction& r);
Fraction operator-(const Fraction& l, const Fraction& r);
bool operator==(const Fraction& l, const Fraction& r);
bool operator<(const Fraction& l, const Fraction& r);
bool operator>(const Fraction& l, const Fraction& r);
