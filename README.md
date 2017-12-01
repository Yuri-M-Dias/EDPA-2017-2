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
* Michelle <?>
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

## Anotações e discussões

**Notas**:
* Deve aceitar elementos repetidos, porém não deve os inserir na estrutura,
apenas contabilizando a inserção;
* Fazer uma média das comparações necessárias para cada inserção;
* Contar quantas colisões foram identificadas, antes de serem tratadas;
* Vetor da hash não precisa ser dinâmico, pode ser declarado estaticamente
vindo do valor de n fornecido.
* Tamanho do vetor em relação ao fator de carga = (n / fator de carga)
* Usar funções de hash diferentes 
(Divisão? Multiplicação? Um relatório diferente para cada uma)

## Estrutura do relatório

**Máximo de 6 páginas! Manter os resultados sucintos!**

* Apresentação, qual é o nosso problema, e como fizemos.
* Resultados para um n qualquer grande (1.000.000.000?),
de uma função hash simples, usando linear e quadrátrica.
Mostrar as métricas/resultados obtidos para cada um deles.
Qual fator de carga usar como padrão? "O melhor caso"(0.7)?
* Testar para diferentes n grandes e pequenos, as duas estruturas,
teste padrão/básico.
* Manter o mesmo n grande, mas métricas com outros fatores de carga:
0,1 - 0,9, para as duas estruturas.
* Manter o n, mas funções de hash diferentes (divisão, multiplicação, bits...)
Não é necessário, porém é interessante realizar os testes para os outros
fatores de carga também.

