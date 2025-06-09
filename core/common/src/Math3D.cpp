#include <common/inc/Math3D.h>

vect3_t<float> Math3D::BarycentricWeights(const vect2_t<float>& a, const vect2_t<float>& b, const vect2_t<float>& c, const vect2_t<float>& p)
{
    const auto ac{c - a};
    const auto ab{b - a};
    const auto pc{c - p};
    const auto pb{b - p};
    const auto ap{p - a};

    const float areaParallelogramABC{ab.cross(ac)};

    if (areaParallelogramABC == 0.0f)
    {
        return {0.0f, 0.0f, 0.0f};
    }

    const float alpha{pb.cross(pc) / areaParallelogramABC};

    const float beta{ap.cross(ac) / areaParallelogramABC};

    const float gamma{1.0f - alpha - beta};

    return {alpha, beta, gamma};
}