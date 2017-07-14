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

  int square_length = m_width-1;
  float current_max_height = m_maxHeight;

  while (square_length > 1)
  {
    // calculate all the midpoints
    for (int sq_i = 0; sq_i < m_width-1; sq_i += square_length)
    {
      for (int sq_j = 0; sq_j < m_width-1; sq_j += square_length)
      {
        CalculateMidpint(current_max_height, sq_i, sq_j, sq_i + square_length, sq_j + square_length);
      }
    }

    // calculate all the side midpoints
    for (int sq_i = 0; sq_i < m_width-1; sq_i += square_length)
    {
      for (int sq_j = 0; sq_j < m_width-1; sq_j += square_length)
      {
        CalculateSidepoint(current_max_height, sq_i, sq_j, sq_i + square_length, sq_j + square_length);
      }
    }
    square_length /= 2;
    current_max_height *= powf(2, -m_roughness);
  }
}

void Fractal::CalculateSidepoint(float max_height, int lt_x, int lt_y, int br_x, int br_y)
{
  // check if a midpoint can be found
  if (br_x-lt_x <=1 || br_y-lt_y<=1)
  {
    return;
  }
  else
  {
    int midpoint_x = (lt_x + br_x) / 2;
    int midpoint_y = (lt_y + br_y) / 2;
    int dist = midpoint_x - lt_x;

    float top_left = (*this)(lt_x, lt_y),
          bottom_left = (*this)(lt_x, br_y),
          top_right = (*this)(br_x, lt_y),
          bottom_right = (*this)(br_x, br_y);

    //random_value = (rand() % (2 * (int)max_height * 100) - max_height * 100) / 100.f;
    float random_value = (rand() % ((int)max_height * 100)) / 100.f;

    // left
    if (lt_x-dist > 0)
      (*this)(lt_x, midpoint_y) = (top_left + bottom_left + (*this)(midpoint_x, midpoint_y) + (*this)(lt_x-dist, midpoint_y)) / 4 + random_value;
    else
      (*this)(lt_x, midpoint_y) = (top_left + bottom_left + (*this)(midpoint_x, midpoint_y)) / 3 + random_value;
    
    // top
    if (lt_y - dist > 0)
      (*this)(midpoint_x, lt_y) = (top_left + top_right + (*this)(midpoint_x, midpoint_y) + (*this)(midpoint_x, lt_y - dist)) / 4 + random_value;
    else
      (*this)(midpoint_x, lt_y) = (top_left + top_right + (*this)(midpoint_x, midpoint_y)) / 3 + random_value;
    
    // right
    if (br_x + dist < m_width-1)
      (*this)(br_x, midpoint_y) = (top_right + bottom_right + (*this)(midpoint_x, midpoint_y) + (*this)(br_x + dist, midpoint_y)) / 4 + random_value;
    else if (br_x == m_width-1)
      (*this)(br_x, midpoint_y) = (top_right + bottom_right + (*this)(midpoint_x, midpoint_y)) / 3 + random_value;
    
    // bottom
    if (br_y + dist < m_width-1)
      (*this)(midpoint_x, br_y) = (bottom_left + bottom_right + (*this)(midpoint_x, midpoint_y)+(*this)(midpoint_x, br_y + dist)) / 4 + random_value;
    else if (br_y == m_width-1)
      (*this)(midpoint_x, br_y) = (bottom_left + bottom_right + (*this)(midpoint_x, midpoint_y)) / 3 + random_value;
  }
}

void Fractal::CalculateMidpint(float max_height, int lt_x, int lt_y, int br_x, int br_y)
{
  if (br_x - lt_x <= 1 || br_y - lt_y <= 1)
  {
    return;
  }
  else
  {
    int midpoint_x = (lt_x + br_x) / 2;
    int midpoint_y = (lt_y + br_y) / 2;

    float top_left = (*this)(lt_x, lt_y),
      bottom_left = (*this)(lt_x, br_y),
      top_right = (*this)(br_x, lt_y),
      bottom_right = (*this)(br_x, br_y);

    float random_value = (rand() % ((int)max_height * 100)) / 100.f;

    // set square array midpoint
    (*this)(midpoint_x, midpoint_y) = (top_left + bottom_left + bottom_right + top_right) / 4 + random_value;
  }
}
