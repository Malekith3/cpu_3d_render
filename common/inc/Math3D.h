
#ifndef MATHCALC_H
#define MATHCALC_H

#include <Vectors.hpp>

namespace Math3D
{

vect3_t<float> BarycentricWeights(const vect2_t<float>& a, const vect2_t<float>& b, const vect2_t<float>& c, const vect2_t<float>& p);

}
#endif //MATHCALC_H
