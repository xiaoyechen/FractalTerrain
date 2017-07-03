#pragma once

#include <math.h>
#include <wchar.h>

#define DEG_RAD_COEFF 0.0174532925f

template<class T>
inline void SafeRelease(T *&ppTToRelease)
{
  if (ppTToRelease != nullptr)
  {
    ppTToRelease->Release();
    ppTToRelease = nullptr;
  }
}

template<class T>
inline void SafeShutdown(T *&ppTToShutdown)
{
  if (ppTToShutdown != nullptr)
  {
    ppTToShutdown->Shutdown();
    delete ppTToShutdown;
    ppTToShutdown = nullptr;
  }
}

template<class T>
inline void SafeDelete(T *&ppTToDelete)
{
  if (ppTToDelete != nullptr)
  {
    delete ppTToDelete;
    ppTToDelete = nullptr;
  }
}