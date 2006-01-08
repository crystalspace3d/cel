#ifndef STABILISER_DISTRIBUTIONS_H
#define STABILISER_DISTRIBUTIONS_H

#include <csutil/array.h>
#include <csutil/ref.h>
#include <csutil/refcount.h>

struct iPcMechanicsObject;

class StabiliserFunction : public csRefCount
{
public:
  virtual ~StabiliserFunction() {};
  virtual float Force(float height) = 0;
};

class IntervalMetaDistribution : public StabiliserFunction
{
public:
  void Add(csRef<StabiliserFunction> func , float low_int , float high_int);

  float Force(float h);
private:
  struct Interval
  {
    float low , high;
    csRef<StabiliserFunction> func;
  };

  csArray<Interval> funcs;
};

class IfFallingDistribution : public StabiliserFunction
{
public:
  IfFallingDistribution(csRef<iPcMechanicsObject> ship,
    csRef<StabiliserFunction> ifdist , csRef<StabiliserFunction> elsedist , float adelta = -1.0);

  float Force(float h);
private:
  csRef<iPcMechanicsObject> ship_mech;
  csRef<StabiliserFunction> if_dist , else_dist;
  float delta;
};

class FallingMultiplierDistribution : public StabiliserFunction
{
public:
  FallingMultiplierDistribution(csRef<iPcMechanicsObject> ship , csRef<StabiliserFunction> adist);

  float Force(float h);
private:
  csRef<iPcMechanicsObject> ship_mech;
  csRef<StabiliserFunction> dist;
};

class ReturnConstantValue : public StabiliserFunction
{
public:
  ReturnConstantValue(float avalue = 0.0);

  float Force(float h);

  float value;
};

class LinearGradient : public StabiliserFunction
{
public:
  LinearGradient(float m , float c) : m(m) , c(c) {}

  float Force(float h) { return m*h + c; }

  float m , c;
};

class InversePowerDistribution : public StabiliserFunction
{
public:
  InversePowerDistribution(float h0 , float f0 , float h1 , float f1);

  float Force(float h);
private:
  static float ComputeK(float h0 , float f0 , float h1 , float f1);
  static float ComputeX(float h , float f , float k);

  float Formula(float h);

  float k , x;
};

class SquareDistribution : public StabiliserFunction
{
public:
  SquareDistribution(float h0 , float f0 , float h1 , float f1);

  float Force(float h);
private:
  float a , k;
};

#endif
