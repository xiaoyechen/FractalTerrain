#pragma once
#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>


class Fractal
{
public:
  Fractal();
  ~Fractal();
  bool Initialize(int, int, int, float);
  bool Generate();
  float& operator()(int, int);
  float operator()(int, int) const;
  void Release();
private:
  void GenerateFractal();
  void CalculateSidepoint(float, int, int, int, int);
  void CalculateMidpint(float, int, int, int, int);
  int m_width;
  int m_maxHeight;
  float m_roughness;
  float* m_fractalMap;
};

