#include "System.h"
#include "resource.h"

int APIENTRY wWinMain(HINSTANCE hinstance, HINSTANCE, LPWSTR, int nCmdShow)
{
  HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

  if (SUCCEEDED(CoInitialize(nullptr)))
  {
    {
      System* sys = new System;
      if (!sys) return 0;

      if (sys->Initialize())
      {
        HACCEL hAccelTable = LoadAccelerators(hinstance, MAKEINTRESOURCE(IDC_FRACTALTERRAIN));
        sys->RunMsgLoop(hAccelTable);
      }

      sys->Shutdown();
      sys = 0;
    }
    CoUninitialize();
  }

  return 0;
}