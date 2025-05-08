#!/bin/bash

# Parar o script em caso de erro
set -e

echo "1. Transformação do VHDL comportamental"
vasy -V -I vhdl -a geradornpa

echo "2. Otimização da função booleana"
boom -A geradornpa geradornpa_otm

echo "3. Prova de equivalência"
proof -a -d geradornpa geradornpa_otm

echo "4. Geração do VHDL estrutural (.vst)"
boog geradornpa_otm geradornpa_est

echo "5. Otimização do .vst"
loon geradornpa_est geradornpa_est_otm

echo "6. Posicionamento das células padrão"
alliance-ocp geradornpa_est_otm gerador_ap

echo "7. Roteamento das células"
nero -p gerador_ap geradornpa_est_otm gerador_ap2

echo "8. Geração de padrões em C"
alliance-genpat -v geradornpa

echo "9. Simulação do circuito"
asimut geradornpa_est_otm lfsr_sim gerador_result

echo "Processo completo!"
