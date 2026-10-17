#pragma once
#include <stdint.h>

struct Color {
private:
    static uint8_t Clamp(int v);
    
public:
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

    bool operator==(const Color& oth) const;
    bool operator!=(const Color& oth) const;
};
