#ifndef _SIM_H_
#define _SIM_H_
#define ACCESS_SIZE 32

#include <stdint.h>
#include <stdbool.h>

struct cache{
    uint32_t assoc; /* associatividade, 1 para map. direto */
    uint32_t block; /* número de bytes por bloco */
    uint32_t num_blocks; /* número de blocos por conjunto */
    uint32_t lat; /* latência de acesso em ciclos */
};


struct stats{
    unsigned long *hits; /* vetor para cada nível de cache */
    unsigned long *misses; /* idem */
    unsigned long cycles; /* número total de ciclos da simulação */
};


typedef struct {
  uint32_t tag;
  uint32_t offset;
  uint32_t index;
} address;


typedef struct{
  uint32_t tag;
  bool valid;
} block;

/** Recebe um traço de endereços de memória acessados e simula hierarquia de memória
  * @param configs vetor de configurações de caches
  * @param num_configs número de níveis de cache
  * @param mem_lat latência em ciclos de acesso à memória
  * @param filename nome do arquivo de traço (ou null)
  * @param stream stream com traço (se filename for null)
  * @return estatísticas de simulação
  */
struct stats * sim(struct cache * configs, int num_configs,
                   uint32_t mem_lat, char * filename, char * stream);

int address_to_index(int address, struct cache config);

int find_index_size(struct cache config);

int find_offset_size(struct cache config);

int find_tag_size(struct cache config);

int extract_tag(uint32_t address, struct cache config);

int extract_offset(uint32_t address, struct cache config);

int extract_index(uint32_t address, struct cache config);

uint32_t hex_string_to_uint32_t(char *hex_string);

address decode_address(char *hex_string, struct cache config);

block *create_memory_level(struct cache config);

#endif // _SIM_H_
