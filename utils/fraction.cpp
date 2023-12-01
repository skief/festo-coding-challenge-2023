#include "fraction.h"
#include <numeric>

void Fraction::simplify(){
    int64_t gcd = std::gcd(num, denom);
    num /= gcd;
    denom /= gcd;

    if (denom < 0){
        num *= -1;
        denom *= 1;
    }
}

Fraction::Fraction():num(0), denom(1){}

Fraction::Fraction(int64_t num, int64_t denom): num(num), denom(denom) {
    simplify();
}

Fraction operator+(const Fraction& l, const Fraction& r){
    return {l.num * r.denom + r.num * l.denom, l.denom * r.denom};
}

Fraction operator-(const Fraction& l, const Fraction& r){
    return {l.num * r.denom - r.num * l.denom, l.denom * r.denom};
}

bool operator==(const Fraction& l, const Fraction& r){
    return (l.num == r.num) && (l.denom == r.denom);
}

bool operator<(const Fraction& l, const Fraction& r){
    uint64_t lcm = std::lcm(l.denom, r.denom);
    uint64_t a = l.num * (lcm / l.denom);
    uint64_t b = r.num * (lcm / r.denom);

    return a < b;
}

bool operator>(const Fraction& l, const Fraction& r){
    uint64_t lcm = std::lcm(l.denom, r.denom);
    uint64_t a = l.num * (lcm / l.denom);
    uint64_t b = r.num * (lcm / r.denom);
    return a > b;
}
