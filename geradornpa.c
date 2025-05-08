#include <stdio.h>
#include <stdlib.h>
#include "genpat.h"

#define MASK_BIT_3      0x08
#define MASK_BIT_4      0x10
#define MASK_BIT_5      0x20
#define MASK_BIT_7      0x80

// Função que simula o LFSR com 8 bits e um seed.
void fsrl(int seed_st);

// Variáveis globais
static int seednumber = 25;
static int i;

// Função auxiliar para converter inteiro para string
char *inttostr(int inteiro) {
    char *str = (char *)malloc(32 * sizeof(char));
    if (str != NULL) {
        sprintf(str, "%d", inteiro);
    }
    return str;
}

int main() {
    int tempo = 0;
    
    // Definir o nome do padrão de teste (genpat)
    DEF_GENPAT("lfsr_sim");

    // Declarar a variável lfsr_state como 8 bits de entrada
    DECLAR("lfsr_state", ":2", "B", IN, "7 downto 0", "");

    for (i = 1; i < 256; i++) {
        fsrl(seednumber);  // Chama a função fsrl para calcular o valor do LFSR

        // Atribui o valor gerado à variável lfsr_state
        AFFECT(inttostr(tempo), "lfsr_state", inttostr(seednumber));

        tempo++;  // Incrementa o tempo para o próximo ciclo
    }

    // Salva o padrão gerado
    SAV_GENPAT();

    return 0;
}

// Função que executa o LFSR com 8 bits e um seed
void fsrl(int seed_st) {
    unsigned char bit7, bit5, bit4, bit3, bit_xor1, bit_xor2, bit_xor3, seed;

    seed = seed_st;

    bit7 = (seed & MASK_BIT_7) >> 7;
    bit5 = (seed & MASK_BIT_5) >> 5;
    bit_xor1 = bit7 ^ bit5;
    bit4 = (seed & MASK_BIT_4) >> 4;
    bit_xor2 = bit4 ^ bit_xor1;
    bit3 = (seed & MASK_BIT_3) >> 3;
    bit_xor3 = bit3 ^ bit_xor2;
    seed = seed >> 1;

    seed = seed | (bit_xor3 << 7);
    seednumber = seed;

    // Aqui o valor gerado de seed será processado pelo genpat
    //printf("| Seed #%d  - Value %d |\n", i, seed);
}

