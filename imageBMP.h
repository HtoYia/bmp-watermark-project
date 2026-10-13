#include <stddef.h>
#include <color.h>

struct ImageBMP {
private:
    size_t weight, height;
    Color** colors;

public:
    size_t GetWeight();
    size_t GetHeight();
    Color& GetPixel(size_t r, size_t c);

    ImageBMP(const Color* const* colors, size_t weight, size_t height);
    ImageBMP(size_t weight, size_t height);
};
