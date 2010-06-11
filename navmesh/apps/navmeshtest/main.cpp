#include <crystalspace.h>
#include "MainApp.h"

CS_IMPLEMENT_APPLICATION

int main (int argc, char *argv[]) {
  return csApplicationRunner<MainApp>::Run (argc, argv);
}
