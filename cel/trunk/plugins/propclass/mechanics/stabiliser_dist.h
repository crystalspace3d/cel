#ifndef STABILISER_DISTRIBUTIONS_H
#define STABILISER_DISTRIBUTIONS_H

#include <csutil/array.h>
#include <csutil/ref.h>
#include <csutil/refcount.h>

struct iPcMechanicsObject;

class celStabiliserFunction : public csRefCount
{
public:
  virtual ~celStabiliserFunction() {};
  virtual float Force(float height) = 0;
};

class celIntervalMetaDistribution : public celStabiliserFunction
{
public:
  void Add(csRef<celStabiliserFunction> func , float low_int , float high_int);

  float Force(float h);
private:
  struct Interval
  {
    float low , high;
    csRef<celStabiliserFunction> func;
  };

  csArray<Interval> funcs;
};

class celIfFallingDistribution : public celStabiliserFunction
{
public:
  celIfFallingDistribution(csRef<iPcMechanicsObject> ship,
    csRef<celStabiliserFunction> ifdist , csRef<celStabiliserFunction> elsedist , float adelta = -1.0);

  float Force(float h);
private:
  csRef<iPcMechanicsObject> ship_mech;
  csRef<celStabiliserFunction> if_dist , else_dist;
  float delta;
};

class celFallingMultiplierDistribution : public celStabiliserFunction
{
public:
  celFallingMultiplierDistribution(csRef<iPcMechanicsObject> ship , csRef<celStabiliserFunction> adist);

  float Force(float h);
private:
  csRef<iPcMechanicsObject> ship_mech;
  csRef<celStabiliserFunction> dist;
};

class celReturnConstantValue : public celStabiliserFunction
{
public:
  celReturnConstantValue(float avalue = 0.0);

  float Force(float h);

  float value;
};

class celLinearGradient : public celStabiliserFunction
{
public:
  celLinearGradient(float m , float c) : m(m) , c(c) {}

  float Force(float h) { return m*h + c; }

  float m , c;
};

class celInversePowerDistribution : public celStabiliserFunction
{
public:
  celInversePowerDistribution(float h0 , float f0 , float h1 , float f1);

  float Force(float h);
private:
  static float ComputeK(float h0 , float f0 , float h1 , float f1);
  static float ComputeX(float h , float f , float k);

  float Formula(float h);

  float k , x;
};

class celSquareDistribution : public celStabiliserFunction
{
public:
  celSquareDistribution(float h0 , float f0 , float h1 , float f1);

  float Force(float h);
private:
  float a , k;
};

#endif
