#pragma once

#include <math.h>

extern const float PI;
extern const float Deg2Rad, Rad2Deg;

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

struct RotatedBounds {
  Vector min = Vector(), max = Vector(), pos = Vector();
  float orient;
  RotatedBounds(Vector mi, Vector ma, Vector po, float o);
  bool collideBasic(Bounds b);
  private:
    float lastOrient;
    Bounds bounds = Bounds();
    void UpdateBounds();
};

struct Ball {
  Vector pos;
  int index;
  int type;
  Ball();
  Ball(Vector p, int i, int t);
};

struct NavNode {
  Vector pos;
  float speed = -1;
  float heading = 361;
  bool reverse = false;
  bool pickup = false;
  Ball ball;
  NavNode();
  NavNode(Vector p, float s = -1, float h = 361, bool rev = false);
  NavNode(Vector p, float s, bool rev, Ball b);
};