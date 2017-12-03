#include <math.h>
#include "sim.h"

int address_to_index(int address, struct cache config){
  int index, aux;
  aux = address/config.block; // endereco / bytes por bloco
  index = aux%(config.num_blocks);

  return index;
}

int find_index_size(struct cache config){
  return (log2(config.num_blocks));
}
