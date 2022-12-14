#pragma once

#include <math.h>

extern const float PI;
extern const float Deg2Rad, Rad2Deg;
//Basic vector class, most functions not used
struct Vector {
  float x, y;
  Vector();
  Vector(float x, float y);
  Vector operator + (Vector v);
  Vector operator += (Vector v);
  Vector operator - (Vector v);
  Vector operator * (float m);
  float operator * (Vector v) ;
  Vector operator / (float m);
  float len();
  float lenSqr();
  float heading();
  bool isZero();
  Vector rotate(float deg);
  Vector project(Vector v);
  Vector reject(Vector v);
  Vector normalize();
  float dot(Vector v);
};
// A grid-aligned bounding box, used for the vision systems
struct Bounds {
  Vector min = Vector(), max = Vector();
  Bounds();
  Bounds(Vector mi, Vector ma);
  Bounds(float minX, float minY, float maxX, float maxY);
  Bounds(Vector center, float width, float height);
  Vector center();
  bool collide(Bounds b);
  bool contains(Bounds b);
  Bounds operator + (Vector v);
};
