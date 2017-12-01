# EDPA-2017-2
[![Build Status](https://travis-ci.org/Yuri-M-Dias/EDPA-2017-2.svg?branch=master)](https://travis-ci.org/Yuri-M-Dias/EDPA-2017-2)

## Como rodar o projeto

Na linha de comando:

``` shell
$ mkdir build && cd build
$ cmake .. -G"CodeBlocks - Unix Makefiles"
$ make
$ ./EDPA20172-hash.o
```
Para executar com um n padrão (5000).

Para customizar o tamanho do n:

``` shell
$ ./EDPA20172-hash.o 4020
```

Nessa compilação, será gerado um .cbp na pasta `build/`, que pode ser importado como um projeto do CodeBlocks.

## Para instalar o software necessário no Ubuntu

``` bash
$ sudo apt-get install build-essential
$ sudo apt-get install cmake
```

## Integrantes
* Caio Sousa <@Caio-Sousa>
* Michelle <?>
* Wellington Galvão <@wgalvao>
* Yuri Matheus <@Yuri-M-Dias>

## Anotações de discussão

### Objetivos do trabalho

* Tabela hash com endereçamento aberto com sondagem linear e sondagem quadrática_

### Funções hash

* Função de verificar qual é o próximo número primo maior do que n
* Função que usa o tamanho do vetor
* Função de verificar qual é o número primo anterior menor do que n

### Fator de carga
* Tamanho do vetor(N) será n / 0,7, com n sendo o tamanho de entrada. Fator de carga = 0,7
* Testar com de 0,1-0,9 de 10 em 10
* 

