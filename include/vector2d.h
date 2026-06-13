#pragma once
#if !defined(AFX_VECTOR2D_H__EC63CCAD_9EB8_450F_A7F0_89A2DCBC2E28__INCLUDED_)
#define AFX_VECTOR2D_H__EC63CCAD_9EB8_450F_A7F0_89A2DCBC2E28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif  // _MSC_VER > 1000

#include <math.h>

#include <fstream>
#define PI 3.14159265
class Vector2D {
  friend std::ifstream& operator>>(std::ifstream& f, Vector2D& v);

 public:
  Vector2D normal();
  Vector2D(float x = 0.0f, float y = 0.0f);
  virtual ~Vector2D();

  float x, y;

  float module();                       // vector module
  float argument();                     // vector argument
  Vector2D unit();                      // returns unit vector
  Vector2D operator-(const Vector2D&);  // vector substract
  Vector2D operator+(const Vector2D&);  // vector sum
  float operator*(const Vector2D&);     // scalar product
  Vector2D operator*(float);            // scalar by a product
};

#endif  // !defined(AFX_VECTOR2D_H__EC63CCAD_9EB8_450F_A7F0_89A2DCBC2E28__INCLUDED_)
