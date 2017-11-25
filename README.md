# EDPA-2017-2

# Como rodar o projeto

Na linha de comando:

``` shell
$ mkdir build && cd build
$ cmake .. -G"CodeBlocks - Unix Makefiles"
$ make
$ ./EDPA20172-hash.o
```
Basta seguir os prompts do programa. *(Precisamos atualizar isso para não precisar de digitar...)*

Nessa compilação, será gerado um .cbp na pasta `build/`, que pode ser importado como um projeto do CodeBlocks.

## Integrantes
* Caio Sousa <@Caio-Sousa>
* Michelle <?>
* Wellington Galvão <@wgalvao>
* Yuri Matheus <@Yuri-M-Dias>

## Anotações de discussão

### Funções hash

* Função de verificar qual é o próximo número primo maior do que n
* Função que usa o tamanho do vetor
* Função de verificar qual é o número primo anterior menor do que n

### Fator de carga
* Tamanho do vetor(N) será 1,3 * n, com n sendo o tamanho de entrada. Fator de carga = 0,7
* Testar com de 0,1-0,9 de 10 em 10
* 

