#include "DataTypes.h"

const float PI = 3.14159265359;
const float Deg2Rad = PI / 180, Rad2Deg = 180 / PI;

Vector::Vector() { x = 0; y = 0; }
Vector::Vector(float x, float y) { this->x = x; this->y = y; }
Vector Vector::operator + (Vector v) {
  Vector res;
  res.x = x + v.x;
  res.y = y + v.y;
  return res;
}
Vector Vector::operator += (Vector v) {
  x += v.x;
  y += v.y;
  return *this;
}
Vector Vector::operator - (Vector v) {
  Vector res;
  res.x = x - v.x;
  res.y = y - v.y;
  return res;
}
Vector Vector::operator * (float m) {
  Vector res;
  res.x = x * m;
  res.y = y * m;
  return res;
}
float Vector::operator * (Vector v) {
  return dot(v);
}
Vector Vector::operator / (float m) {
  Vector res;
  res.x = x / m;
  res.y = y / m;
  return res;
}
float Vector::len() {
  return sqrt(lenSqr());
}
float Vector::lenSqr() {
  return x * x + y * y;
}
float Vector::heading() {
  return atan2f(y, x);
}
Vector Vector::rotate(float rad) {
  Vector v = Vector(x * cosf(rad) - y * sinf(rad),
                    y * cosf(rad) + x * sinf(rad));
  return v;
}
bool Vector::isZero() {
  return x == 0 && y == 0;
}
Vector Vector::normalize() {
  return *this / len();
}
float Vector::dot(Vector v) {
  return x * v.x + y * v.y;
}
Vector Vector::project(Vector v) {
  if (isZero() || v.isZero())
      return Vector();
  return v * (dot(v) / v.lenSqr());
}
Vector Vector::reject(Vector v) {
  if (isZero() || v.isZero())
      return Vector();
  return *this - v * (dot(v) / v.lenSqr());
}

Bounds::Bounds() { min = Vector(); max = Vector(); }
Bounds::Bounds(Vector mi, Vector ma) { min = mi; max = ma; }
Bounds::Bounds(float minX, float minY, float maxX, float maxY) { min.x = minX; min.y = minY; max.x = maxX; max.y = maxY; }
Bounds::Bounds(Vector center, float width, float height) { min = center; min.x -= width/2; min.y -= height/2; max = center; max.x += width/2; max.y += height/2; }
Vector Bounds::center() {
  return Vector(max.x + min.x,
                max.y + min.y) / 2;
}
bool Bounds::collide(Bounds b) {
  return ((min.x > b.min.x && min.x < b.max.x) || (max.x > b.min.x && max.x < b.max.x)) &&
          ((min.y > b.min.y && min.y < b.max.y) || (max.y > b.min.y && max.y < b.max.y));
}
bool Bounds::contains(Bounds b) {
  return b.min.x > min.x && b.max.x < max.x &&
          b.min.y > min.y && b.max.y < max.y;
}
Bounds Bounds::operator + (Vector v) {
  Bounds b = Bounds();
  b.min = min + v;
  b.max = max + v;
  return b;
}
