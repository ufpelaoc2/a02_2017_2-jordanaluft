#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

uint32_t hex_string_to_uint32_t(char *hex_string){
  char buffer[11] = "0x";
  uint32_t out;
  strcat(buffer, hex_string);
  sscanf(buffer, "%X", &out);
  return out;
}

address decode_address(char *hex_string, struct cache config){
  uint32_t hex = hex_string_to_uint32_t(hex_string);
  address new_address;
  new_address.tag = extract_tag(hex, config);
  new_address.offset = extract_offset(hex, config);
  new_address.index = extract_index(hex, config);
  return new_address;
}

block *create_memory_level(struct cache config){
  return malloc(config.num_blocks*(sizeof(block)));
}

bool level_read(block *level, struct cache config, char *hex_string){
  // Retorna true se bloco é valido e tag é igual, retorna false caso
  // contrário
  address a = decode_address(hex_string, config);
  block b = level[a.index];
  if (!b.valid)
    return false;
  if (b.tag == a.tag)
    return true;
  return false;
}

void level_write(block *level, struct cache config, char *hex_string){
  address a = decode_address(hex_string, config);
  block b = {a.tag, true};
  level[a.index] = b;
}

block** create_h_memory(struct cache *configs, int num_configs){
  block** h_memory = malloc(sizeof(block*) * num_configs);

  for(int i=0; i < num_configs; i++)
    h_memory[i] = create_memory_level(configs[i]);

  return h_memory;
}
