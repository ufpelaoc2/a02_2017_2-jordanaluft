#include "sim.h"
#include <stdio.h>


int address_to_index(int address, struct cache config){
  int index, aux;
  aux = address/config.block; // endereco / bytes por bloco
  index = aux%(config.num_blocks);

  return index;
}
