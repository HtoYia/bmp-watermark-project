
#include <gtest/gtest.h>
#include <vector>

#include "imageBMP.h"
#include "color.h"


class ImageBMPTest : public ::testing::Test {
protected:
    Color red;
    Color green;
    Color blue;

    ImageBMPTest()
        : red(255, 0, 0),
          green(0, 255, 0),
          blue(0, 0, 255) {
    }
};


//
// Constructors
//

TEST_F(ImageBMPTest, DefaultConstructor) {
    ImageBMP image;

    EXPECT_EQ(image.GetWidth(), 0);
    EXPECT_EQ(image.GetHeight(), 0);
}

TEST_F(ImageBMPTest, ConstructorWithDimensions) {
    ImageBMP image(3, 2);

    EXPECT_EQ(image.GetWidth(), 3);
    EXPECT_EQ(image.GetHeight(), 2);
}

TEST_F(ImageBMPTest, ConstructorWithColors) {
    std::vector<Color> colors = {
        red, green, blue,
        blue, red, green
    };

    ImageBMP image(3, 2, colors);

    EXPECT_EQ(image.GetWidth(), 3);
    EXPECT_EQ(image.GetHeight(), 2);

    EXPECT_EQ(image.Pixel(0, 0), red);
    EXPECT_EQ(image.Pixel(0, 1), green);
    EXPECT_EQ(image.Pixel(0, 2), blue);

    EXPECT_EQ(image.Pixel(1, 0), blue);
    EXPECT_EQ(image.Pixel(1, 1), red);
    EXPECT_EQ(image.Pixel(1, 2), green);
}


//
// Working with single pixels
//

TEST_F(ImageBMPTest, ReadPixel) {
    std::vector<Color> colors = {red, green, blue};
    ImageBMP image(3, 1, colors);

    EXPECT_EQ(image.Pixel(0, 0), red);
    EXPECT_EQ(image.Pixel(0, 1), green);
    EXPECT_EQ(image.Pixel(0, 2), blue);
}

TEST_F(ImageBMPTest, ModifyPixel) {
    ImageBMP image(2, 2);

    image.Pixel(0, 0) = red;
    image.Pixel(0, 1) = green;
    image.Pixel(1, 0) = blue;

    EXPECT_EQ(image.Pixel(0, 0), red);
    EXPECT_EQ(image.Pixel(0, 1), green);
    EXPECT_EQ(image.Pixel(1, 0), blue);
}

TEST_F(ImageBMPTest, ModifyPixelDoesNotAffectOthers) {
    ImageBMP image(2, 2);

    image.Pixel(0, 0) = red;
    image.Pixel(0, 1) = green;

    image.Pixel(0, 0) = blue;

    EXPECT_EQ(image.Pixel(0, 0), blue);
    EXPECT_EQ(image.Pixel(0, 1), green);
}

TEST_F(ImageBMPTest, ConstPixelAccess) {
    std::vector<Color> colors = {red, green};
    const ImageBMP image(2, 1, colors);

    EXPECT_EQ(image.Pixel(0, 0), red);
    EXPECT_EQ(image.Pixel(0, 1), green);
}


//
// Image sizes
//

TEST_F(ImageBMPTest, ZeroWidth) {
    ImageBMP image(0, 5);

    EXPECT_EQ(image.GetWidth(), 0);
    EXPECT_EQ(image.GetHeight(), 5);
}

TEST_F(ImageBMPTest, ZeroHeight) {
    ImageBMP image(5, 0);

    EXPECT_EQ(image.GetWidth(), 5);
    EXPECT_EQ(image.GetHeight(), 0);
}

TEST_F(ImageBMPTest, SinglePixelImage) {
    ImageBMP image(1, 1);

    image.Pixel(0, 0) = red;

    EXPECT_EQ(image.GetWidth(), 1);
    EXPECT_EQ(image.GetHeight(), 1);
    EXPECT_EQ(image.Pixel(0, 0), red);
}
