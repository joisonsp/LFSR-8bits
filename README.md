# Gerador de Números Pseudo-Aleatórios (PRNG) com LFSR de 8 bits

Este repositório contém a implementação de um Gerador de Números Pseudo-Aleatórios (PRNG) de 8 bits utilizando um Registrador de Deslocamento com Realimentação Linear (LFSR). O projeto inclui o código VHDL para a descrição do hardware do LFSR e um código em C para gerar os padrões de teste (testbench) para simulação e verificação do design.

O principal objetivo deste projeto é fornecer um exemplo funcional de um LFSR de 8 bits, demonstrando sua arquitetura, o polinômio de realimentação utilizado e como ele pode ser simulado e verificado.

## Características

*   **Tipo de Gerador:** PRNG baseado em LFSR (arquitetura Fibonacci).
*   **Largura do Registrador:** 8 bits.
*   **Polinômio de Realimentação:** `x⁸ + x⁶ + x⁵ + x⁴ + 1`. Este é um polinômio primitivo para LFSRs de 8 bits, o que garante um período máximo da sequência gerada.
*   **Período da Sequência:** 2⁸ - 1 = 255 estados únicos (excluindo o estado zero).
*   **Semente Inicial (Seed):** O código de teste em C utiliza a semente inicial `0xB8` (hexadecimal), que corresponde a `10111000` em binário ou 184 em decimal.

## Arquivos do Repositório

*   `geradorNPA.vhdl` (ou nome similar): Contém a descrição em VHDL do LFSR de 8 bits.
    *   Define a entidade `LFSR` com portas para clock (`clk`), reset (`rst`), estado inicial/semente (`lfsr_state` como entrada, embora o comportamento principal seja de geração interna), e a saída do LFSR (`lfsr_out`).
    *   A arquitetura `Behavioral` implementa a lógica de deslocamento e a realimentação XOR baseada no polinômio `x⁸ + x⁶ + x⁵ + x⁴ + 1`.
*   `geradorNPA.c` (ou nome similar, conforme fornecido no anexo `pasted_content.txt`): Contém o código em C utilizado para gerar os padrões de teste para o LFSR.
    *   Este código simula o comportamento do LFSR de 8 bits com o mesmo polinômio e semente (`0xB8`).
    *   Utiliza a biblioteca `genpat.h` (parte do Alliance CAD System ou similar) para criar um arquivo de vetores de teste (geralmente com extensão `.pat`), que pode ser usado por simuladores VHDL para verificar a correção do design de hardware.

## Como Funciona

O LFSR implementado é do tipo Fibonacci. A cada pulso de clock:
1.  Os bits no registrador são deslocados uma posição (geralmente para a direita).
2.  Um novo bit é calculado fazendo o XOR de bits específicos do estado atual do registrador (os "taps"). Para o polinômio `x⁸ + x⁶ + x⁵ + x⁴ + 1`, os taps são nas posições correspondentes aos expoentes (considerando a convenção de que o bit de saída do último flip-flop é x⁸, e a entrada do primeiro é x⁰, os taps são nas saídas dos flip-flops que representam x⁸, x⁶, x⁵, x⁴).
    *   No código VHDL fornecido (`lfsr_state(7 downto 0)`), a realimentação `XOR_result <= lfsr_state(7) xor lfsr_state(5) xor lfsr_state(4) xor lfsr_state(3)` é inserida como o novo bit mais significativo, e os bits `lfsr_state(7 downto 1)` são deslocados.
    *   No código C fornecido, os taps são definidos como `LFSR_TAP_BIT1 4`, `LFSR_TAP_BIT2 5`, `LFSR_TAP_BIT3 6`, `LFSR_TAP_BIT4 8`. A função `updateLFSR` calcula o feedback `((*state>>(3)) & 1) ^ ((*state>>(4)) & 1) ^ ((*state>>(5)) & 1) ^ ((*state>>(7)) & 1)` (considerando `state[0]` como LSB) e o insere no MSB (bit 7) após o deslocamento.
3.  O bit calculado é inserido na primeira posição do registrador.
4.  A saída do LFSR pode ser o valor completo do registrador ou um bit específico (geralmente o bit que é deslocado para fora).

A semente inicial não deve ser zero, pois isso faria com que o LFSR permanecesse no estado zero.

## Fluxo de Simulação e Síntese

Os arquivos fornecidos são tipicamente usados em um fluxo de design de hardware digital:

1.  **Geração de Padrões de Teste:**
    *   Compile o arquivo `geradorNPA.c` (que inclui `genpat.h`) usando um compilador C (como o `gcc`) e a ferramenta `alliance-genpat` (se estiver usando o Alliance CAD System) ou um processo similar para gerar o arquivo de padrões de teste (ex: `lfsr_sim.pat`).
    *   Este arquivo `.pat` conterá a sequência de saída esperada do LFSR para a semente `0xB8`.

2.  **Simulação VHDL:**
    *   Utilize um simulador VHDL (como GHDL, ModelSim, ou o `asimut` do Alliance CAD System).
    *   Compile o arquivo `geradorNPA.vhdl`.
    *   Simule o design VHDL utilizando o arquivo de padrões de teste (`.pat`) gerado na etapa anterior para verificar se a saída do circuito VHDL corresponde à saída esperada.

3.  **Síntese (Opcional, para implementação física):**
    *   Se o objetivo for implementar o LFSR em hardware (FPGA ou ASIC), o código VHDL pode ser sintetizado usando ferramentas de síntese apropriadas (como Xilinx Vivado, Intel Quartus, ou as ferramentas do Alliance como `vasy`, `boom`, `boog`, `loon`).
    *   O processo de síntese converterá a descrição VHDL em uma netlist de componentes lógicos que podem ser mapeados para a tecnologia alvo.
    *   Após a síntese, seguem-se etapas de place & route e verificação temporal.
