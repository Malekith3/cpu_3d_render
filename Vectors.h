#ifndef VECTORS_H
#define VECTORS_H

#include <cmath>

#include <cmath> // for fabs
#include <type_traits> // for std::enable_if

template <typename T>
struct vect2_t
{
  T x{0};
  T y{0};

  template <typename Scalar>
  vect2_t operator*(Scalar scalar) const
  {
    static_assert(std::is_arithmetic<Scalar>::value, "Scalar must be a numeric type");
    return {x * scalar, y * scalar};
  }

  vect2_t operator+(const vect2_t& other) const
  {
    return {x + other.x, y + other.y};
  }

  T dot(const vect2_t& other) const
  {
    return x * other.x + y * other.y;
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const vect2_t<T>& vec)
{
  os << "vect2_t(" << vec.x << ", " << vec.y << ")";
  return os;
}

template <typename T>
struct vect3_t
{
  T x{0};
  T y{0};
  T z{0};

  template <typename Scalar>
  vect3_t operator*(Scalar scalar) const
  {
    static_assert(std::is_arithmetic<Scalar>::value, "Scalar must be a numeric type");
    return {x * scalar, y * scalar, z * scalar};
  }

  vect3_t operator+(const vect3_t& other) const
  {
    return {x + other.x, y + other.y, z + other.z};
  }

  T dot(const vect3_t& other) const
  {
    return x * other.x + y * other.y + z * other.z;
  }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const vect3_t<T>& vec)
{
  os << "vect3_t(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  return os;
}
#endif //VECTORS_H
