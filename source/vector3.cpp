#include <cmath>
#include "vector3.h"

Vector3::Vector3() 
  : x(0.0f), y(0.0f), z(0.0f) { }

Vector3::Vector3(float _x, float _y, float _z) 
  : x(_x), y(_y), z(_z) { }

bool Vector3::Equals(const Vector3& that, float epsilon) const
{
  return fabs(x - that.x) + 
         fabs(y - that.y) +
         fabs(z - that.z) < epsilon;
}

float Vector3::SqrLength() const
{
  return x * x + y * y + z * z;
}

float Vector3::Length() const
{
  return sqrt(SqrLength());
}

Vector3 Vector3::Unit() const
{
  return *this / this->Length();
}

Vector3& Vector3::operator+=(const Vector3& that)
{
  x += that.x;
  y += that.y;
  z += that.z;

  return *this;
}

Vector3& Vector3::operator-=(const Vector3& that)
{
  return *this += -that;
}

Vector3 Cross(const Vector3& a, const Vector3& b)
{
  return Vector3(a.y * b.z - a.z * b.y,
                 a.z * b.x - a.x * b.z,
                 a.x * b.y - a.y * b.x);
}

float Dot(const Vector3& a, const Vector3& b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 operator-(const Vector3& vector)
{
  return Vector3(-vector.x, -vector.y, -vector.z);
}

Vector3 operator+(Vector3 left, const Vector3& right)
{
  left += right;
  return left;
}

Vector3 operator-(Vector3 left, const Vector3& right)
{
  return left + -right;
}

Vector3 operator*(const Vector3& vector, float scalar)
{
  return Vector3(vector.x * scalar,
                 vector.y * scalar,
                 vector.z * scalar);
}

Vector3 operator/(const Vector3& vector, float scalar)
{
  return vector * (1.0f / scalar);
}

bool operator==(const Vector3& left, const Vector3& right)
{
  return left.x == right.x &&
         left.y == right.y &&
         left.z == right.z;
}
