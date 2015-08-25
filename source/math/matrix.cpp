#include "types.h"
#include "vec.h"
#include "matrix.h"

mat4x4f translation(float x, float y, float z)
{
  return mat4x4f
  {
    { 1.0f,    0,    0,    x },
    {    0, 1.0f,    0,    y },
    {    0,    0, 1.0f,    z },
    {    0,    0,    0, 1.0f }
  };
}

mat4x4f scale(float x, float y, float z)
{
  return mat4x4f
  {
    {    x,    0,    0,    0 },
    {    0,    y,    0,    0 },
    {    0,    0,    z,    0 },
    {    0,    0,    0, 1.0f }
  };
}

mat4x4f scale(float s)
{
  return scale(s, s, s);
}

mat4x4f lookAt(vec3f eye, vec3f up, vec3f target)
{
  vec3f zaxis = (eye - target).unit();
  vec3f xaxis = cross(up, zaxis).unit();
  vec3f yaxis = cross(zaxis, xaxis);

  return mat4x4f
  {
    { xaxis.x(), xaxis.y(), xaxis.z(), -dot(eye, xaxis) },
    { yaxis.x(), yaxis.y(), yaxis.z(), -dot(eye, yaxis) },
    { zaxis.x(), zaxis.y(), zaxis.z(), -dot(eye, zaxis) },
    {         0,         0,         0,                1 }
  };
}

mat4x4f projection(float vfov, float aspect, float near, float far)
{
  // field of view
  float y = 1.0f / tan(vfov * float(M_PI) / 360.0f);
  float x = y / aspect;

  // map z to [0, 1]
  float zz = -far / (far - near);
  float zw = -far * near / (far - near);

  return mat4x4f
  {
    { x,     0,     0,     0 },
    { 0,     y,     0,     0 },
    { 0,     0,    zz,    zw },
    { 0,     0, -1.0f,     0 }
  };
}
