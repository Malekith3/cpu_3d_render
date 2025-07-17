#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <common/inc/Math3D.h>

#include "doctest/doctest.h"

class BarycentricTestFixture
{
public:
    const vect2_t<float> a{0.0f, 0.0f};
    const vect2_t<float> b{1.0f, 0.0f};
    const vect2_t<float> c{0.0f, 1.0f};
    static constexpr float EPSILON = 1e-4f;

    static void checkWeights(const vect3_t<float>& weights, const vect3_t<float>& expected)
    {
        CHECK(weights.x == doctest::Approx(expected.x).epsilon(EPSILON));
        CHECK(weights.y == doctest::Approx(expected.y).epsilon(EPSILON));
        CHECK(weights.z == doctest::Approx(expected.z).epsilon(EPSILON));
    }
};

TEST_CASE_FIXTURE(BarycentricTestFixture, "Barycentric weights at triangle vertices")
{

    SUBCASE("Point at vertex A")
    {
        vect2_t<float> point(0.0f, 0.0f);
        auto weights = Math3D::BarycentricWeights(a, b, c, point);
        vect3_t<float> expected(1.0f, 0.0f, 0.0f);

        checkWeights(weights, expected);
        CHECK(weights.sum() == doctest::Approx(1.0f).epsilon(EPSILON));
    }

    SUBCASE("Point at vertex B")
    {
        vect2_t<float> point(1.0f, 0.0f);
        auto weights = Math3D::BarycentricWeights(a, b, c, point);
        vect3_t<float> expected(0.0f, 1.0f, 0.0f);

        checkWeights(weights, expected);
        CHECK(weights.sum() == doctest::Approx(1.0f).epsilon(EPSILON));
    }

    SUBCASE("Point at vertex C")
    {
        vect2_t<float> point(0.0f, 1.0f);
        auto weights = Math3D::BarycentricWeights(a, b, c, point);
        vect3_t<float> expected(0.0f, 0.0f, 1.0f);

        checkWeights(weights, expected);
        CHECK(weights.sum() == doctest::Approx(1.0f).epsilon(EPSILON));
    }
}

TEST_CASE_FIXTURE(BarycentricTestFixture, "Barycentric weights at triangle centroid")
{
    constexpr vect2_t<float> point(1.0f / 3.0f, 1.0f / 3.0f);
    auto weights = Math3D::BarycentricWeights(a, b, c, point);
    constexpr vect3_t<float> expected(1.0f / 3.0f, 1.0f / 3.0f, 1.0f / 3.0f);

    checkWeights(weights, expected);
    CHECK(weights.sum() == doctest::Approx(1.0f).epsilon(EPSILON));
}

TEST_CASE_FIXTURE(BarycentricTestFixture, "Barycentric weights at edge midpoints")
{

    SUBCASE("Edge midpoint AB")
    {
        vect2_t<float> point(0.5f, 0.0f);
        auto weights = Math3D::BarycentricWeights(a, b, c, point);
        vect3_t<float> expected(0.5f, 0.5f, 0.0f);

        checkWeights(weights, expected);
        CHECK(weights.sum() == doctest::Approx(1.0f).epsilon(EPSILON));
    }

    SUBCASE("Edge midpoint AC")
    {
        vect2_t<float> point(0.0f, 0.5f);
        auto weights = Math3D::BarycentricWeights(a, b, c, point);
        vect3_t<float> expected(0.5f, 0.0f, 0.5f);

        checkWeights(weights, expected);
        CHECK(weights.sum() == doctest::Approx(1.0f).epsilon(EPSILON));
    }

    SUBCASE("Edge midpoint BC")
    {
        vect2_t<float> point(0.5f, 0.5f);
        auto weights = Math3D::BarycentricWeights(a, b, c, point);
        vect3_t<float> expected(0.0f, 0.5f, 0.5f);

        checkWeights(weights, expected);
        CHECK(weights.sum() == doctest::Approx(1.0f).epsilon(EPSILON));
    }
}

TEST_CASE("Barycentric weights for degenerate triangle")
{
    vect2_t<float> a(0.0f, 0.0f);
    vect2_t<float> b(1.0f, 0.0f);
    vect2_t<float> c(2.0f, 0.0f);
    vect2_t<float> point(0.5f, 0.0f);

    auto weights = Math3D::BarycentricWeights(a, b, c, point);

    CHECK(weights.x == 0.0f);
    CHECK(weights.y == 0.0f);
    CHECK(weights.z == 0.0f);
    CHECK(weights.sum() == 0.0f);
}

TEST_CASE("Barycentric weights properties")
{
    vect2_t<float> a(0.0f, 0.0f);
    vect2_t<float> b(2.0f, 0.0f);
    vect2_t<float> c(1.0f, 2.0f);

    SUBCASE("Weights sum to 1 for points inside triangle")
    {
        std::vector<vect2_t<float>> testPoints = {
            {0.5f, 0.5f},
            {1.0f, 0.5f},
            {0.8f, 0.3f},
            {1.2f, 0.8f}
        };

        for (const auto& point : testPoints)
        {
            auto weights = Math3D::BarycentricWeights(a, b, c, point);
            CHECK(weights.sum() == doctest::Approx(1.0f).epsilon(1e-4f));
        }
    }

    SUBCASE("Point reconstruction using weights")
    {
        vect2_t<float> point(1.0f, 0.5f);
        auto [x, y, z] = Math3D::BarycentricWeights(a, b, c, point);

        float reconstructed_x = x * a.x + y * b.x + z * c.x;
        float reconstructed_y = x * a.y + y * b.y + z * c.y;

        CHECK(reconstructed_x == doctest::Approx(point.x).epsilon(1e-4f));
        CHECK(reconstructed_y == doctest::Approx(point.y).epsilon(1e-4f));
    }
}

TEST_CASE("Barycentric weights edge cases")
{

    SUBCASE("Zero area triangle (duplicate vertices)")
    {
        vect2_t<float> a(1.0f, 1.0f);
        vect2_t<float> b(1.0f, 1.0f);
        vect2_t<float> c(2.0f, 2.0f);
        vect2_t<float> point(1.5f, 1.5f);

        auto [x, y, z] = Math3D::BarycentricWeights(a, b, c, point);

        CHECK(x == 0.0f);
        CHECK(y == 0.0f);
        CHECK(z == 0.0f);
    }

}
