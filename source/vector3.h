#pragma once

class Vector3
{
public:
  float x, y, z;

  Vector3();
  Vector3(float x, float y, float z);

  bool Equals(const Vector3& that, float epsilon) const;

  float SqrLength() const;
  float Length() const;
  Vector3 Unit() const;

  Vector3& operator+=(const Vector3& that);
  Vector3& operator-=(const Vector3& that);
};

Vector3 Cross(const Vector3& a, const Vector3& b);
float Dot(const Vector3& a, const Vector3& b);

Vector3 operator-(const Vector3& vector);
Vector3 operator+(Vector3 left, const Vector3& right);
Vector3 operator-(Vector3 left, const Vector3& right);
Vector3 operator*(const Vector3& vector, float scalar);
Vector3 operator/(const Vector3& vector, float scalar);
bool operator==(const Vector3& left, const Vector3& right);

