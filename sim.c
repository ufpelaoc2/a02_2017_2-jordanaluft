#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "sim.h"

struct stats * sim(struct cache * configs, int num_configs,
                   uint32_t mem_lat, char * filename, char * stream) {
  block** h_memory = create_h_memory(configs, num_configs);
  struct stats *stats = create_stats(num_configs);

  char mode;
  char address[9];

  if (filename){
    FILE *fp = fopen(filename, "r");
    if (!fp){
      printf("Erro ao abrir arquivo\n");
      return NULL;
    }
    while(!feof(fp)){
      fscanf(fp, "%c %s\n", &mode, address);
      run_simulation(h_memory, stats, configs, num_configs,
                     mem_lat, mode, address);
    }
    fclose(fp);
  } else {
    for(int i = 0; stream[i] != '\0'; i += 11){
      sscanf(stream + i, "%c %s\n", &mode, address);
      run_simulation(h_memory, stats, configs, num_configs,
                     mem_lat, mode, address);
    }
  }

  return stats;
}

bool run_simulation(block** h_memory, struct stats *stats,
                    struct cache *configs, int num_configs,
                    uint32_t mem_lat, char mode, char *address) {

  if (mode == 'R'){
    bool found = h_memory_read(h_memory, configs, num_configs, stats, address);
    if (!found){
      stats->cycles += mem_lat;
      h_memory_write_on_buffer(h_memory, configs, num_configs, address);
    }
  }
  else if (mode == 'W'){
    bool found = h_memory_read_on_buffer(h_memory, configs, num_configs, address);
    if (!found){
      h_memory_write(h_memory, configs, stats, num_configs, address);
      stats->cycles += mem_lat;
    }
    else
      stats->cycles += configs->lat;
  }
  else
    return false;
  return true;
}

bool h_memory_read(block** h_memory, struct cache *configs,
                   int num_configs, struct stats *stats,
                   char *hex_string) {

  for(int i=0; i < num_configs; i++){
    bool found = level_read(h_memory[i], configs[i], hex_string);
    stats->cycles += configs[i].lat;
    if (found){
      stats->hits[i] += 1;
      return true; // encontrou o que procurava
    }
    else
      stats->misses[i] += 1;
  }
  return false;
}

void h_memory_write(block** h_memory, struct cache *configs,
		    struct stats *stats,
                    int num_configs, char *hex_string) {

  for(int i=0; i < num_configs; i++){
    level_write(h_memory[i], configs[i], hex_string);
    stats->cycles += configs[i].lat;
  }
}

bool h_memory_read_on_buffer(block** h_memory, struct cache *configs,
                             int num_configs, char *hex_string) {

  for(int i=0; i < num_configs; i++){
    bool found = level_read(h_memory[i], configs[i], hex_string);
    if (found)
      return true;
  }
  return false;
}

void h_memory_write_on_buffer(block** h_memory, struct cache *configs,
                              int num_configs, char *hex_string) {

  for(int i=0; i < num_configs; i++)
    level_write(h_memory[i], configs[i], hex_string);
}

bool level_read(block *level, struct cache config, char *hex_string) {
  // Retorna true se bloco é valido e tag é igual, retorna false caso
  // contrário
  address addr = decode_address(hex_string, config);

  int index;
  if(is_associative(config)){
    group grp = make_group(config, addr);
    for(int i = grp.start; i < grp.end; i++){
      index = 0;
    }
  }
  else
    index = addr.index;

  block b = level[index];
  if (!b.valid)
    return false;
  if (b.tag == addr.tag){
    level[index].timestamp = timestamp();
    return true;
  }
  return false;
}

void level_write(block *level, struct cache config, char *hex_string) {
  address a = decode_address(hex_string, config);
  block b = {a.tag, true, timestamp()};
  int index;
  if(is_associative(config))
    index = 0;
  else
    index = a.index;

  level[index] = b;
}

block** create_h_memory(struct cache *configs, int num_configs) {
  block** h_memory = malloc(sizeof(block*) * num_configs);

  for(int i=0; i < num_configs; i++)
    h_memory[i] = create_memory_level(configs[i]);

  return h_memory;
}

block *create_memory_level(struct cache config){
  return malloc(config.num_blocks*(sizeof(block)));
}

struct stats *create_stats(int num_configs) {
  struct stats *stats = malloc(sizeof(struct stats));
  stats->cycles = 0;

  stats->hits = malloc(sizeof(unsigned long) * num_configs);
  stats->misses = malloc(sizeof(unsigned long) * num_configs);

  return stats;
}

int find_index_size(struct cache config) {
  int b = is_associative(config) ? config.assoc : config.num_blocks;
  return (log2(b));
}

int find_offset_size(struct cache config) {
  return (log2(config.block));
}

int find_tag_size(struct cache config) {
  int index_size = find_index_size(config);
  int offset_size = find_offset_size(config);
  int tag_size = ACCESS_SIZE - (index_size + offset_size);
  return tag_size;
}

int extract_tag(uint32_t address, struct cache config) {
  int index_size = find_index_size(config);
  int offset_size = find_offset_size(config);
  int size = index_size + offset_size;
  return (address >> size);
}

int extract_offset(uint32_t address, struct cache config) {
  int index_size = find_index_size(config);
  int tag_size = find_tag_size(config);
  int size = index_size + tag_size;
  uint32_t aux = (address << size);
  return (aux >> size);
}

int extract_index(uint32_t address, struct cache config) {
  int offset_size = find_offset_size(config);
  int tag_size = find_tag_size(config);
  uint32_t aux = (address << tag_size);
  int size = tag_size + offset_size;
  return (aux >> size);
}

uint32_t hex_string_to_uint32_t(char *hex_string) {
  char buffer[11] = "0x";
  uint32_t out;
  strcat(buffer, hex_string);
  sscanf(buffer, "%X", &out);
  return out;
}

address decode_address(char *hex_string, struct cache config) {
  uint32_t hex = hex_string_to_uint32_t(hex_string);
  address new_address;
  new_address.tag = extract_tag(hex, config);
  new_address.offset = extract_offset(hex, config);
  new_address.index = extract_index(hex, config);
  return new_address;
}

int timestamp(){
  static int i = 0;
  return ++i;
}

bool is_associative(struct cache config){
  return config.assoc > 1;
}

group make_group(struct cache config, address addr){
  int blocks_per_group = ceil((double)config.num_blocks / (double)config.assoc);
  int start = blocks_per_group * addr.index;
  int end = start + blocks_per_group;
  group g = {start, end};
  return g;
};
