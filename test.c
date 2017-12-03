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

int main(){
  test_address_to_index();
  return 0;
}
