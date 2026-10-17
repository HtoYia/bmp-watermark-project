#include <gtest/gtest.h>
#include "color.h"

// Constructors

TEST(ColorTest, DefaultIsBlack) {
    Color c;
    EXPECT_EQ(c.R, 0);
    EXPECT_EQ(c.G, 0);
    EXPECT_EQ(c.B, 0);
}

TEST(ColorTest, StoresChannels) {
    Color c(10, 20, 30);
    EXPECT_EQ(c.R, 10);
    EXPECT_EQ(c.G, 20);
    EXPECT_EQ(c.B, 30);
}

TEST(ColorTest, MissingArgumentsAreZero) {
    Color c(7, 8);
    EXPECT_EQ(c.R, 7);
    EXPECT_EQ(c.G, 8);
    EXPECT_EQ(c.B, 0);
}

TEST(ColorTest, BoundaryValuesAreStored) {
    Color c(0, 128, 255);
    EXPECT_EQ(c.R, 0);
    EXPECT_EQ(c.G, 128);
    EXPECT_EQ(c.B, 255);
}

TEST(ColorTest, CopyIsIndependent) {
    Color a(1, 2, 3);
    Color b = a;
    b.R = 99;
    EXPECT_EQ(a.R, 1);
    EXPECT_EQ(b.R, 99);
}

// Operators == and !=

TEST(ColorTest, EqualColors) {
    EXPECT_TRUE(Color(1, 2, 3) == Color(1, 2, 3));
    EXPECT_FALSE(Color(1, 2, 3) != Color(1, 2, 3));
}

TEST(ColorTest, DifferentInEachChannel) {
    EXPECT_TRUE(Color(1, 2, 3) != Color(9, 2, 3));
    EXPECT_TRUE(Color(1, 2, 3) != Color(1, 9, 3));
    EXPECT_TRUE(Color(1, 2, 3) != Color(1, 2, 9));
    EXPECT_FALSE(Color(1, 2, 3) == Color(9, 2, 3));
}

// Operators +, -, *, / for Color and Color

TEST(ColorTest, AddColor) {
    Color c = Color(10, 20, 30) + Color(1, 2, 3);
    EXPECT_EQ(c.R, 11);
    EXPECT_EQ(c.G, 22);
    EXPECT_EQ(c.B, 33);
}

TEST(ColorTest, AddColorOverflowClampsTo255) {
    Color c = Color(250, 255, 0) + Color(10, 1, 0);
    EXPECT_EQ(c.R, 255);
    EXPECT_EQ(c.G, 255);
    EXPECT_EQ(c.B, 0);
}

TEST(ColorTest, SubtractColor) {
    Color c = Color(10, 20, 30) - Color(1, 2, 3);
    EXPECT_EQ(c.R, 9);
    EXPECT_EQ(c.G, 18);
    EXPECT_EQ(c.B, 27);
}

TEST(ColorTest, SubtractColorUnderflowClampsTo0) {
    Color c = Color(1, 0, 5) - Color(2, 1, 5);
    EXPECT_EQ(c.R, 0);
    EXPECT_EQ(c.G, 0);
    EXPECT_EQ(c.B, 0);
}

TEST(ColorTest, MultiplyColor) {
    Color c = Color(2, 3, 4) * Color(5, 6, 7);
    EXPECT_EQ(c.R, 10);
    EXPECT_EQ(c.G, 18);
    EXPECT_EQ(c.B, 28);
}

TEST(ColorTest, MultiplyColorOverflowClampsTo255) {
    Color c = Color(16, 16, 0) * Color(16, 15, 5);
    EXPECT_EQ(c.R, 255);
    EXPECT_EQ(c.G, 240);
    EXPECT_EQ(c.B, 0);
}

TEST(ColorTest, DivideColor) {
    Color c = Color(100, 50, 9) / Color(10, 5, 3);
    EXPECT_EQ(c.R, 10);
    EXPECT_EQ(c.G, 10);
    EXPECT_EQ(c.B, 3);
}

TEST(ColorTest, DivideColorTruncatesAndKeepsZero) {
    Color c = Color(7, 1, 0) / Color(2, 2, 2);
    EXPECT_EQ(c.R, 3);
    EXPECT_EQ(c.G, 0);
    EXPECT_EQ(c.B, 0);
}

TEST(ColorTest, DivideMaxByOne) {
    Color c = Color(255, 255, 255) / Color(1, 1, 1);
    EXPECT_EQ(c.R, 255);
    EXPECT_EQ(c.G, 255);
    EXPECT_EQ(c.B, 255);
}

// Operators +, -, *, / for Color and uint8_t

TEST(ColorTest, AddScalar) {
    Color c = Color(10, 20, 30) + 5;
    EXPECT_EQ(c.R, 15);
    EXPECT_EQ(c.G, 25);
    EXPECT_EQ(c.B, 35);
}

TEST(ColorTest, AddScalarOverflowClampsTo255) {
    Color c = Color(250, 255, 0) + 10;
    EXPECT_EQ(c.R, 255);
    EXPECT_EQ(c.G, 255);
    EXPECT_EQ(c.B, 10);
}

TEST(ColorTest, SubtractScalar) {
    Color c = Color(10, 20, 30) - 5;
    EXPECT_EQ(c.R, 5);
    EXPECT_EQ(c.G, 15);
    EXPECT_EQ(c.B, 25);
}

TEST(ColorTest, SubtractScalarUnderflowClampsTo0) {
    Color c = Color(3, 0, 255) - 5;
    EXPECT_EQ(c.R, 0);
    EXPECT_EQ(c.G, 0);
    EXPECT_EQ(c.B, 250);
}

TEST(ColorTest, MultiplyScalar) {
    Color c = Color(1, 2, 3) * 4;
    EXPECT_EQ(c.R, 4);
    EXPECT_EQ(c.G, 8);
    EXPECT_EQ(c.B, 12);
}

TEST(ColorTest, MultiplyScalarOverflowClampsTo255) {
    Color c = Color(100, 64, 0) * 4;
    EXPECT_EQ(c.R, 255);
    EXPECT_EQ(c.G, 255);
    EXPECT_EQ(c.B, 0);
}
