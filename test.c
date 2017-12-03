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
  THEN("offset: 12");

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
  return 0;
}
