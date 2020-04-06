#include <pch.h>

#include "../include/generation.h"
#include "../include/delta.h"

TEST(PerlinNoiseTests, SanityCheck) {
    aw::PerlinNoiseGenerator generator;
    generator.setSeed(0x0);

    float a = generator.perlin(10.0f, 10.0f);
    float b = generator.perlin(10.0f, 10.0f);

    EXPECT_EQ(a, b);
}

TEST(PerlinNoiseTests, SanityCheckOffGrid) {
    aw::PerlinNoiseGenerator generator;
    generator.setSeed(0x0);

    float a = generator.perlin(10.2f, 10.4f);
    float b = generator.perlin(10.2f, 10.4f);

    EXPECT_EQ(a, b);

    a = generator.perlin(-10.2f, 10.4f);
    b = generator.perlin(-10.2f, 10.4f);

    EXPECT_EQ(a, b);
}

TEST(PerlinNoiseTests, SanityCheckBigNumber) {
    aw::PerlinNoiseGenerator generator;
    generator.setSeed(0x0);

    double a = generator.perlin(10000000.3, 10000000.3);
    double b = generator.perlin(10000000.3, 10000000.3);

    EXPECT_EQ(a, b);
}
