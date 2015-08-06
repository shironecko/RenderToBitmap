#pragma once

template<typename TComponent>
struct Vector3
{
public:
  TComponent x;
  TComponent y;
  TComponent z;

  inline Vector3() {};
  inline Vector3(TComponent x, TComponent y, TComponent z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
};
