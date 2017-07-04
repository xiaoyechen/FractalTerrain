#include "Fractal.h"

Fractal::Fractal()
  :m_fractalMap(nullptr)
{
}


Fractal::~Fractal()
{
}

bool Fractal::Initialize(int w, int h, int max_height, float roughness)
{
  // side size must be 2^n+1
  if (w != h || remainder(log(w-1)/log(2),1) != 0)
    return false;

  m_width = w;
  m_maxHeight = max_height;
  m_roughness = roughness;

  m_fractalMap = new float[m_width*m_width];

  return true;
}

bool Fractal::Generate()
{
  GenerateFractal();

  return true;
}

float & Fractal::operator()(int i, int j)
{
  if (i >= 0 && i < m_width && j >= 0 && i < m_width)
  {
    return m_fractalMap[i*m_width + j];
  }
}

float Fractal::operator()(int i, int j) const
{
  if (i >= 0 && i < m_width && j >= 0 && i < m_width)
  {
    return m_fractalMap[i*m_width + j];
  }
}

void Fractal::Release()
{
  if (m_fractalMap)
  {
    delete[] m_fractalMap;
  }
}

void Fractal::GenerateFractal()
{
  // set 4 corner points to 0
  (*this)(0, 0) = 0;
  (*this)(0, m_width - 1) = 0;
  (*this)(m_width-1, 0) = 0;
  (*this)(m_width - 1, m_width - 1) = 0;

  srand(time(NULL));

  GenerateFractalRecursive(m_maxHeight, 0, 0, m_width - 1, m_width - 1);
}

void Fractal::GenerateFractalRecursive(float max_height, int lt_x, int lt_y, int br_x, int br_y)
{
  int midpoint_x = (lt_x + br_x) / 2;
  int midpoint_y = (lt_y + br_y) / 2;

  // check if the midpoint is valid
  if (midpoint_x == lt_x || midpoint_x == br_x ||
    midpoint_y == lt_y || midpoint_y == br_y)
  {
    return;
  }
  else
  {
    float top_left = (*this)(lt_x, lt_y),
          bottom_left = (*this)(lt_x, br_y),
          top_right = (*this)(br_x, lt_y),
          bottom_right = (*this)(br_x, br_y);

    //float random_value = (rand() % (2 * (int)max_height * 100) - max_height * 100) / 100.f;
    float random_value = (rand() % ((int)max_height * 100)) / 100.f;
    // set square array midpoint
    (*this)(midpoint_x, midpoint_y) = (top_left + bottom_left +bottom_right+top_right) / 4 + random_value;

  //  ClampTerrainHeight((*this)(midpoint_x, midpoint_y), -max_height, max_height);

    // set square's 4 sides midpoints
    //random_value = (rand() % (2 * (int)max_height * 100) - max_height * 100) / 100.f;
    random_value = (rand() % ((int)max_height * 100)) / 100.f;

    (*this)(lt_x, midpoint_y) = (top_left + bottom_left + (*this)(midpoint_x, midpoint_y)) / 3 + random_value;// *powf(2, -m_roughness);
    (*this)(midpoint_x, lt_y) = (top_left + top_right + (*this)(midpoint_x, midpoint_y)) / 3 + random_value;// *powf(2, -m_roughness);
    (*this)(br_x, midpoint_y) = (top_right + bottom_right + (*this)(midpoint_x, midpoint_y)) / 3 + random_value;// *powf(2, -m_roughness);
    (*this)(midpoint_x, br_y) = (bottom_left + bottom_right + (*this)(midpoint_x, midpoint_y)) / 3 + random_value;// *powf(2, -m_roughness);

    //ClampTerrainHeight((*this)(lt_x, midpoint_y), 0, max_height);
    //ClampTerrainHeight((*this)(midpoint_x, lt_y), 0, max_height);
   // ClampTerrainHeight((*this)(br_x, midpoint_y), 0, max_height);
    //ClampTerrainHeight((*this)(midpoint_x, br_y), 0, max_height);

    // generate fractal for the 4 sub-squares
    float new_max_height = max_height*powf(2, -m_roughness);
    GenerateFractalRecursive(new_max_height, lt_x, lt_y, midpoint_x, midpoint_y);
    GenerateFractalRecursive(new_max_height, lt_x, midpoint_y, midpoint_x, br_y);
    GenerateFractalRecursive(new_max_height, midpoint_x, lt_y, br_x, midpoint_y);
    GenerateFractalRecursive(new_max_height, midpoint_x, midpoint_y, br_x, br_y);
  }
}

void Fractal::ClampTerrainHeight(float &val, const float &min_val, const float &max_val)
{
  if (val < min_val)
    val = min_val;
  else if (val > max_val)
    val = max_val;
}


