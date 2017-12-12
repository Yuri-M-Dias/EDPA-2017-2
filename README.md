# EDPA-2017-2
[![Build Status](https://travis-ci.org/Yuri-M-Dias/EDPA-2017-2.svg?branch=master)](https://travis-ci.org/Yuri-M-Dias/EDPA-2017-2)

## Como rodar o projeto

Na linha de comando:

```shell
$ mkdir build && cd build
$ cmake .. -G"CodeBlocks - Unix Makefiles"
$ make
$ ./EDPA20172-hash.o
```
Para executar com um n padrão (5000).

Para customizar o tamanho do n:

```shell
$ ./EDPA20172-hash.o 4020
```

Nessa compilação, será gerado um .cbp na pasta `build/`,
 que pode ser importado como um projeto do CodeBlocks.

## Para instalar o software necessário no Ubuntu/Debian

```shell
$ sudo apt-get install build-essential
$ sudo apt-get install cmake
```

## Integrantes
* Caio Sousa <@Caio-Sousa>
* Michelle <@mchrisso>
* Wellington Galvão <@wgalvao>
* Yuri Matheus <@Yuri-M-Dias>

## Objetivos do trabalho

**Foco**:
Tabela hash com endereçamento aberto com sondagem linear e sondagem quadrática

**Partes do relatório**:
1. quantidade de vezes que cada chave foi gerada;
2. quantidade de chaves geradas iguais a chaves já presentes nas estruturas;
3. tempo total de processamento (CPU) relativo à construção de cada estrutura;
4. fator de carga (se for o caso);
5. número de comparações de chaves em cada estrutura;
