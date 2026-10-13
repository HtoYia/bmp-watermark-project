#include <stdint.h>

struct Color {
    uint8_t R, G, B;

    Color& operator+(const Color& oth);
    Color& operator-(const Color& oth);
    Color& operator*(const Color& oth);
    Color& operator/(const Color& oth);

    Color& operator+(uint8_t val);
    Color& operator-(uint8_t val);
    Color& operator*(uint8_t val);
    Color& operator/(uint8_t val);
};
