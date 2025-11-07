#include <EzGraph.h>
#include "library.h"
int menu_key = 1;

int main(){

  EzSetKeyHandler(key_menu);
  draw_menu();
  EzShowBuffer();
  EzEventLoop();
  return 0;
}
