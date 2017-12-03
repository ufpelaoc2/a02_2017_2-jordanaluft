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

int main(){
  test_address_to_index();
  test_find_index_size();
  test_find_offset_size();
  test_find_tag_size();
  test_extract_tag();
  test_extract_offset();
  return 0;
}
