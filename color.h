#pragma once
#include <stdint.h>

struct Color {
    uint8_t R, G, B;

    Color(uint8_t R=0, uint8_t G=0, uint8_t B=0);

    Color operator+(const Color& oth) const;
    Color operator-(const Color& oth) const;
    Color operator*(const Color& oth) const;
    Color operator/(const Color& oth) const;

    Color& operator+=(const Color& oth);
    Color& operator-=(const Color& oth);
    Color& operator*=(const Color& oth);
    Color& operator/=(const Color& oth);

    Color operator+(uint8_t oth) const;
    Color operator-(uint8_t oth) const;
    Color operator*(uint8_t oth) const;
    Color operator/(uint8_t oth) const;

    Color& operator+=(uint8_t val);
    Color& operator-=(uint8_t val);
    Color& operator*=(uint8_t val);
    Color& operator/=(uint8_t val);
};
