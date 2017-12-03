#include <stdbool.h>
#include "simplegrade.h"
#include "sim.h"


void test_address_to_index(){
  DESCRIBE("address_to_index");

  WHEN("address: 1200, num_blocks: 64, blocks: 16");
  THEN("index = 11");

  struct cache config = {1, 16, 64, 1};

  int esperado = 11;
  int observado =  address_to_index(1200, config);
  isEqual(esperado, observado, 1);
}

void test_find_index_size(){
  DESCRIBE("find_index_size");

  WHEN("num_blocos: 64");
  THEN("index: 6");

  struct cache config = {1, 1, 64, 1};

  int esperado = 6;
  int observado = find_index_size(config);
  isEqual(esperado, observado, 1);
}

void test_find_offset_size(){
  DESCRIBE("find_offset_size");

  WHEN("blocks: 16");
  THEN("offset: 4");

  struct cache config = {1, 16, 1, 1};

  int esperado = 4;
  int observado = find_offset_size(config);
  isEqual(esperado, observado, 1);
}

void test_find_tag_size(){
  DESCRIBE("find_tag_size");

  WHEN("index size: 6 and offset size: 4");
  THEN("tag: 22");

  struct cache config = {1, 16, 64, 1};

  int esperado = 22;
  int observado = find_tag_size(config);
  isEqual(esperado, observado, 1);
}

void test_extract_tag(){
  DESCRIBE("extract_tag");

  WHEN("address: 4294966282"); // 11111111111111111111110000001010
  THEN("tag: 4194303"); // 1111111111111111111111

  uint32_t address = 4294966282;
  struct cache config = {1, 16, 64, 1};
  int esperado = 4194303;
  int observado = extract_tag(address, config);
  isEqual(esperado, observado, 1);
}

void test_extract_offset(){
  DESCRIBE("extract_offset");

  WHEN("address: 4294966282");
  THEN("offset: 10");

  struct cache config = {1, 16, 64, 1};
  uint32_t address = 4294966282;
  int esperado = 10;
  int observado = extract_offset(address, config);
  isEqual(esperado, observado, 1);
}

void test_extract_index(){
  DESCRIBE("extract_offset");

  WHEN("address: 4294966474"); // 11111111111111111111110011001010
  THEN("index: 12");

  struct cache config = {1, 16, 64, 1};
  uint32_t address = 4294966474;
  int esperado = 12;
  int observado = extract_index(address, config);
  isEqual(esperado, observado, 1);
}

void test_hex_string_to_uint32_t(){
  DESCRIBE("convert_address");

  WHEN("hex_string: 000000FF");
  THEN("255");

  char *hex_string = "000000FF";
  int esperado = 255;
  int observado = hex_string_to_uint32_t(hex_string);
  isEqual(esperado, observado, 1);
}

void test_decode_address(){
  DESCRIBE("decode_address");

  WHEN("address: FFFFFC0A");
  THEN("index: 0, offset: 10, tag: 4194303");

  char *hex = "FFFFFC0A";
  struct cache config = {1, 16, 64, 1};

  address esperado;
  esperado.tag = 4194303;
  esperado.offset = 10;
  esperado.index = 0;

  address observado = decode_address(hex, config);
  isEqual(esperado.tag, observado.tag, 1);
  isEqual(esperado.offset, observado.offset, 1);
  isEqual(esperado.index, observado.index, 1);
}

void test_create_memory_level(){
  DESCRIBE("create_memory_level");

  WHEN("num_blocks: 64");
  THEN("Não quebra");

  struct cache config = {1, 6, 64, 1};

  block *array_address = create_memory_level(config);
  isNotNull(array_address, 1);
}

void test_level_read(){
  DESCRIBE("Level read. Return True if hit, false if miss.");

  WHEN("Cold");
  THEN("Return false");

  char *hex_string = "00000000";
  struct cache config = {1, 16, 64, 1};
  block *level = create_memory_level(config);
  bool observado = level_read(level, config, hex_string);
  bool esperado = false;
  isEqual(observado, esperado, 1);

  WHEN("Hot and Tag Not Equal");
  THEN("Return false");

  block b = {4194304, true};
  level[0] = b;
  observado = level_read(level, config, "FFFFFC0A");

  esperado = false;
  isEqual(observado, esperado, 1);

  WHEN("Hot and Tag Equal");
  THEN("Return True");

  b.tag = 4194303;
  level[0] = b;
  esperado = true;
  observado = level_read(level, config, "FFFFFC0A");
  isEqual(observado, esperado, 1);
}

void test_level_write(){
  DESCRIBE("level write");

  WHEN("Written");
  THEN("Return True");

  char *hex_string = "FFFFFC0A";
  struct cache config = {1, 16, 64, 1};
  block *level = create_memory_level(config);

  level_write(level, config, hex_string);
  bool observado = level_read(level, config, hex_string);
  bool esperado = true;
  isEqual(esperado, observado, 1);
}

void test_create_h_memory(){
  DESCRIBE("create h_memory");

  WHEN("Allocate");
  THEN("Return Not Null");

  int num_configs = 2;
  struct cache config1 = {1, 16, 64, 1};
  struct cache config2 = {1, 10, 20, 1};
  struct cache configs[] = {config1, config2};

  block** h_memory = create_h_memory(configs, num_configs);
  isNotNull(h_memory, 1);

  for(int i=0; i < num_configs; i++)
    isNotNull(h_memory[i], 1);
}

void test_h_memory_write(){
  DESCRIBE("h_memory write");

  WHEN("Written");
  THEN("Return True");

  char *hex_string = "FFFFFC0A";
  int num_configs = 2;
  struct cache config1 = {1, 16, 64, 1};
  struct cache config2 = {1, 8, 16, 1};
  struct cache configs[] = {config1, config2};

  block** h_memory = create_h_memory(configs, num_configs);

  h_memory_write(h_memory, configs, num_configs, hex_string);
  isEqual(h_memory[0][0].valid, true, 1);
  isEqual(h_memory[1][1].valid, true, 1);
  isEqual(h_memory[0][1].valid, false, 1);
}

void test_create_stats(){
  DESCRIBE("create stats");

  struct stats *stats = create_stats(2);
  isNotNull(stats, 1);
  isEqual(stats->cycles, 0, 1);
  isNotNull(stats->hits, 1);
  isNotNull(stats->misses, 1);
}

int main(){
  test_address_to_index();
  test_find_index_size();
  test_find_offset_size();
  test_find_tag_size();
  test_extract_tag();
  test_extract_offset();
  test_extract_index();
  test_hex_string_to_uint32_t();
  test_decode_address();
  test_create_memory_level();
  test_level_read();
  test_level_write();
  test_create_h_memory();
  test_h_memory_write();
  test_create_stats();
  return 0;
}
