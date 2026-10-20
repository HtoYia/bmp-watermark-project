
#include <gtest/gtest.h>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "bmpHandler.h"
#include "imageBMP.h"

namespace fs = std::filesystem;

namespace {
const std::filesystem::path kTestCases = TEST_CASES_DIR;

struct TestCase {
    std::string name;
    bool input_top_down;
};

const std::vector<TestCase> kCases = {
    {"small_2x2", false},
    {"odd_width_3x2", false},
    {"top_down_2x3", true},
};


// Images comparation

void ExpectImagesEqual(const ImageBMP& expected,
                       const ImageBMP& actual) {
    ASSERT_EQ(expected.GetWidth(), actual.GetWidth());
    ASSERT_EQ(expected.GetHeight(), actual.GetHeight());

    for (size_t r = 0; r < expected.GetHeight(); ++r) {
        for (size_t c = 0; c < expected.GetWidth(); ++c) {
            EXPECT_EQ(expected.Pixel(r, c), actual.Pixel(r, c))
                << "Pixel at row=" << r << ", col=" << c;
        }
    }
}

fs::path CasePath(const std::string& name,
                  const std::string& file) {
    return kTestCases / name / file;
}

}


// StructHandler::TryRead

TEST(StructHandlerTest, ReadExactData) {
    const fs::path path = "./test_read.bin";

    const char source[] = {'A', 'B', 'C', 'D'};
    {
        std::ofstream out(path, std::ios::binary);
        ASSERT_TRUE(out.is_open());
        out.write(source, sizeof(source));
    }

    char result[sizeof(source)] = {};
    {
        std::ifstream in(path, std::ios::binary);
        ASSERT_TRUE(in.is_open());

        EXPECT_TRUE(StructHandler::TryRead(
            in, result, sizeof(result)));
    }

    EXPECT_EQ(std::string(result, sizeof(result)),
              std::string(source, sizeof(source)));

    fs::remove(path);
}

TEST(StructHandlerTest, ReadFailsWhenDataIsTooShort) {
    const fs::path path = "./test_short.bin";

    {
        std::ofstream out(path, std::ios::binary);
        out.write("AB", 2);
    }

    char result[4] = {};
    std::ifstream in(path, std::ios::binary);

    ASSERT_TRUE(in.is_open());
    EXPECT_FALSE(StructHandler::TryRead(in, result, sizeof(result)));

    fs::remove(path);
}


// StructHandler::TryWrite

TEST(StructHandlerTest, WriteExactData) {
    const fs::path path = "./test_write.bin";
    const char source[] = {'1', '2', '3', '4'};

    {
        std::ofstream out(path, std::ios::binary);
        ASSERT_TRUE(out.is_open());

        EXPECT_TRUE(StructHandler::TryWrite(
            out, source, sizeof(source)));
    }

    char result[sizeof(source)] = {};
    {
        std::ifstream in(path, std::ios::binary);
        ASSERT_TRUE(in.is_open());

        in.read(result, sizeof(result));
        ASSERT_EQ(in.gcount(),
                  static_cast<std::streamsize>(sizeof(result)));
    }

    EXPECT_EQ(std::string(result, sizeof(result)),
              std::string(source, sizeof(source)));

    fs::remove(path);
}


// HandlerBMP::Read

TEST(HandlerBMPTest, ReadAllSampleImages) {
    for (const auto& test : kCases) {
        SCOPED_TRACE(test.name);

        ImageBMP actual;
        const std::string path =
            CasePath(test.name, "input.bmp").string();

        ASSERT_TRUE(HandlerBMP::Read(path.c_str(), actual));

        EXPECT_GT(actual.GetWidth(), 0);
        EXPECT_GT(actual.GetHeight(), 0);
    }
}

TEST(HandlerBMPTest, ReadFailsForMissingFile) {
    ImageBMP image;

    EXPECT_FALSE(HandlerBMP::Read(
        "./test_cases/file_does_not_exist.bmp", image));
}

TEST(HandlerBMPTest, ReadFailsForInvalidFile) {
    const fs::path path = "./test_invalid.bmp";

    {
        std::ofstream out(path, std::ios::binary);
        out << "This is not a BMP file";
    }

    ImageBMP image;
    EXPECT_FALSE(HandlerBMP::Read(path.string().c_str(), image));

    fs::remove(path);
}


// HandlerBMP::Write

TEST(HandlerBMPTest, WriteAndReadBackBothOrientations) {
    for (const auto& test : kCases) {
        SCOPED_TRACE(test.name);

        ImageBMP expected;
        const std::string input =
            CasePath(test.name, "input.bmp").string();

        ASSERT_TRUE(HandlerBMP::Read(input.c_str(), expected));

        for (bool top_down : {false, true}) {
            const fs::path output =
                CasePath(test.name, "test_output.bmp");

            ASSERT_TRUE(HandlerBMP::Write(
                expected, output.string().c_str(), top_down));

            ImageBMP actual;
            ASSERT_TRUE(HandlerBMP::Read(
                output.string().c_str(), actual));

            ExpectImagesEqual(expected, actual);
        }
    }
}

TEST(HandlerBMPTest, OutputMatchesExpectedSample) {
    for (const auto& test : kCases) {
        SCOPED_TRACE(test.name);

        ImageBMP expected;
        ImageBMP input;

        const std::string expected_path =
            CasePath(test.name, "output.bmp").string();
        const std::string input_path =
            CasePath(test.name, "input.bmp").string();

        ASSERT_TRUE(HandlerBMP::Read(
            expected_path.c_str(), expected));
        ASSERT_TRUE(HandlerBMP::Read(
            input_path.c_str(), input));

        ExpectImagesEqual(expected, input);
    }
}

TEST(HandlerBMPTest, WriteFailsForInvalidPath) {
    ImageBMP image(2, 2);

    EXPECT_FALSE(HandlerBMP::Write(
        image, "./missing_directory/output.bmp", false));
}
