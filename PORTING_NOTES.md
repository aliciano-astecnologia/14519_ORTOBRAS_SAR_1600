# Porte da logica 11928 para STM8S

## Escopo

A logica de produto do firmware PIC16F1826 foi portada para a camada de
aplicacao do projeto STM8S003K3. A SPL/HAL, o BSP, a comunicacao da botoeira,
a criptografia, o watchdog, o display e a persistencia em Data EEPROM foram
mantidos.

O buzzer e a rotina de teste de hardware do firmware PIC nao fazem parte deste
porte.

## Mapeamento adotado

| Funcao 11928 | Recurso STM8S |
| --- | --- |
| Sensor contador | Entrada 4 / CN2.5 |
| Limite lateral | Entrada 5 / CN2.11 |
| Limite guardado/fecha | Entrada 6 / CN2.10 |
| Limite de subida | Entrada 7 / CN2.9 |
| Sinal do onibus | Entrada 8 / CN2.8 |
| Tecla interna | Entrada 9 |
| Botao sobe | Comunicacao da botoeira |
| Botao desce | Comunicacao da botoeira |
| Botao fecha | Comunicacao da botoeira |
| Rele sobe | Saida 1 / CN2.2 |
| Rele desce | Saida 2 / CN2.3 |
| Rele fecha | Saida 3 / CN2.4 |

As entradas 4 a 7 sao normalizadas em `App.h`, pois o BSP da placa as le como
ativas em nivel baixo.

## Comportamento portado

- O sinal do onibus habilita o produto e aciona o rele fecha.
- Subida e descida exigem pressao continua na respectiva tecla.
- O limite de subida impede e interrompe a subida.
- Toda troca de comando desliga as bobinas por 50 ms antes de habilitar a nova
  direcao.
- Um ciclo e registrado quando o limite de subida e atingido durante a subida.
- O display normalmente mostra ciclos totais.
- A tecla interna mostra ciclos desde a manutencao depois de 200 ms.
- Manter a tecla interna pressionada por 10 s zera apenas o contador desde a
  manutencao.
- Qualquer tecla da botoeira continua renovando o timeout de 30 s do display.

## Pontos para validacao em bancada

1. Confirmar o mapeamento fisico das entradas 4, 5 e 6 com o chicote 11928.
2. Confirmar as polaridades dos quatro sensores no conector.
3. Confirmar que a saida 3 esta ligada ao rele fecha no hardware final.
4. Ensaiar perda da comunicacao da botoeira durante subida e descida.
5. Ensaiar o limite de subida e verificar um unico incremento por ciclo.

## Build verificado

Os modulos `App.c` e `AppCiclos.c` foram compilados com Cosmic STM8 V4.4.9.
O link completo e a conversao para S19 terminaram sem erros.

