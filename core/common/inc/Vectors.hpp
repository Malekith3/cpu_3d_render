#ifndef VECTORS_H
#define VECTORS_H

#include <cmath> // for fabs
#include <type_traits> // for std::enable_if
#include <ostream>

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

  template <typename Scalar>
  friend vect2_t operator*(Scalar scalar, const vect2_t &v)
  {
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar must be a numeric type");
    return {scalar * v.x, scalar * v.y};
  }

  vect2_t operator+(const vect2_t& other) const
  {
    return {x + other.x, y + other.y};
  }

  vect2_t operator-(const vect2_t& other) const  // Subtraction operator
  {
    return {x - other.x, y - other.y};
  }

  T dot(const vect2_t& other) const
  {
    return x * other.x + y * other.y;
  }

  vect2_t rotate(T angle) const
  {
      T cosA = std::cos(angle);
      T sinA = std::sin(angle);
      return {x * cosA - y * sinA, x * sinA + y * cosA};
  }

  T cross(const vect2_t &other) const { return x * other.y - y * other.x; }

  T magnitude() const
  {
    return std::sqrt(x * x + y * y);
  }

  T magnitudeSquared() const
  {
    return x * x + y * y;
  }

  vect2_t normalize() const
  {
    auto mag = magnitude();
    return *this * (1/mag);
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

  template <typename Scalar>
  friend vect3_t operator*(Scalar scalar, const vect3_t &v)
  {
    static_assert(std::is_arithmetic_v<Scalar>, "Scalar must be a numeric type");
    return {scalar * v.x, scalar * v.y, scalar*v.z};
  }

  vect3_t operator+(const vect3_t& other) const
  {
    return {x + other.x, y + other.y, z + other.z};
  }

  vect3_t operator-(const vect3_t& other) const  // Subtraction operator
  {
    return {x - other.x, y - other.y, z - other.z};
  }

  T dot(const vect3_t& other) const
  {
    return x * other.x + y * other.y + z * other.z;
  }

  vect3_t cross(const vect3_t& other) const
  {
    return {
      y * other.z - z * other.y,
      z * other.x - x * other.z,
      x * other.y - y * other.x
    };
  }

  vect3_t rotateX(T angleRad) const
  {
      T cosA = std::cos(angleRad);
      T sinA = std::sin(angleRad);
      return {x, y * cosA - z * sinA, y * sinA + z * cosA};
  }

  vect3_t rotateY(T angleRad) const
  {
    T cosA = std::cos(angleRad);
    T sinA = std::sin(angleRad);
    return {x * cosA + z * sinA, y, -x * sinA + z * cosA};
  }

  vect3_t rotateZ(T angleRad) const
  {
    T cosA = std::cos(angleRad);
    T sinA = std::sin(angleRad);
    return {x * cosA - y * sinA, x * sinA + y * cosA, z};
  }

  T magnitude() const
  {
    return std::sqrt(x * x + y * y + z * z);
  }

  vect3_t normalize() const
  {
    auto mag = magnitude();
    return *this * (1/mag);
  }

  T magnitudeSquared() const
  {
    return x * x + y * y + z * z;
  }

  T sum() const
  {
    return x + y + z;
  }

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const vect3_t<T>& vec)
{
  os << "vect3_t(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  return os;
}

#endif //VECTORS_H
