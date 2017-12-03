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

int find_offset_size(struct cache config){
  return (log2(config.block));
}

int find_tag_size(struct cache config){
  int index_size = find_index_size(config);
  int offset_size = find_offset_size(config);
  int tag_size = ACCESS_SIZE - (index_size + offset_size);
  return tag_size;
}

int extract_tag(uint32_t address, struct cache config){
  int index_size = find_index_size(config);
  int offset_size = find_offset_size(config);
  int size = index_size + offset_size;
  return (address >> size);
}

int extract_offset(uint32_t address, struct cache config){
  int index_size = find_index_size(config);
  int tag_size = find_tag_size(config);
  int size = index_size + tag_size;
  uint32_t aux = (address << size);
  return (aux >> size);
}

int extract_index(uint32_t address, struct cache config){
  int offset_size = find_offset_size(config);
  int tag_size = find_tag_size(config);
  uint32_t aux = (address << tag_size);
  int size = tag_size + offset_size;
  return (aux >> size);
}
