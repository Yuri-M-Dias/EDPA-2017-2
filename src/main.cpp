/*
 * Projeto final da Matéria EDPA, de 2017-2, do INF-UFG, nível Mestrado.
 * Tabela Hash: Endereçamento aberto com sondagem linear e quadrática.
 * Integrantes: Caio César, Michelle Oliveira, Yuri Matheus Dias
 * e Wellington Galvão.
 */

#include <cstdio>
#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <cstring>
#include <iomanip>

#include "timeAndRNG.h"

using namespace std;

extern const long RNG_MAX;
extern const long RNG_MIN;

/* Definições */

struct EstatisticasChave {
    EstatisticasChave() {
        this->quantidadeGerada = 0;
        this->colisoes = 0;
    };
    unsigned long colisoes;
    unsigned long quantidadeGerada;
};

struct EstatisticasEstrutura {
    EstatisticasEstrutura() {
        this->repetidos = 0;
        this->comparacoes = 0;
        this->falhas = 0;
    };
    unsigned long repetidos;
    unsigned long comparacoes;
    // Apenas ocorre na quadrática
    unsigned long falhas;
};

unsigned long *populateArrayWithRandomNumbers(unsigned long size);

void printTimeDiff(double start, double end);

bool numeroEprimo(int n);

int proximoPrimo(int n);

int geraNumeroRandomico();

void hashInsereLinear(int elemento, unsigned long *T, unsigned long tamanhoVetorHash,
                      EstatisticasChave *estatisticasChaves, EstatisticasEstrutura &estatisticasEstrutura);

void hashInsereQuadratica(int elemento, unsigned long *T, unsigned long tamanhoVetorHash,
                          EstatisticasChave *estatisticasChaves, EstatisticasEstrutura &estatisticasEstrutura);

int calculaHash(int chave, int tamanho);

void insercaoLinear(unsigned long *vetorNumerosAleatorios, unsigned long numeroItens, float fatorDeCarga,
                    bool usarProxPrimo = false);

void insercaoQuadratica(unsigned long *vetorNumerosAleatorios, unsigned long numeroItens, float fatorDeCarga,
                        bool usarProxPrimo = false);

unsigned long calculaTamanhoVetorUsandoProxNumeroPrimo(unsigned long numeroItens, unsigned long tamanhoVetor);

/* Main */

const int VALOR_FLAG_VAZIO = -1;
const float LIMITE_FATOR_DE_CARGA = 1;

int main(int argc, char *argv[]) {
    unsigned long n = 100000;

    cout << setprecision(10);

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <tamanho do n>" << endl;
        cerr << "Usando n com valor padrao: " << n << "" << endl;
    } else {
        n = stoi(argv[1]);
    }

    cout << "Gerando " << n << " numeros aleatorios entre [" << RNG_MIN << "] e [" << RNG_MAX << "]" << endl;

    unsigned long *vetorNumerosAleatorios = populateArrayWithRandomNumbers(n);

    cout << "Numeros aleatórios gerados." << endl;

    for (float fatorDeCarga = 0.0; fatorDeCarga <= LIMITE_FATOR_DE_CARGA; fatorDeCarga += 0.1) {
        insercaoLinear(vetorNumerosAleatorios, n, fatorDeCarga);
        insercaoQuadratica(vetorNumerosAleatorios, n, fatorDeCarga);
        insercaoLinear(vetorNumerosAleatorios, n, fatorDeCarga, true);
        insercaoQuadratica(vetorNumerosAleatorios, n, fatorDeCarga, true);
    }

    return 0;
}

unsigned long calculaTamanhoVetor(unsigned long numeroItens, float fatorDeCarga) {
    unsigned long tamanhoVetor = numeroItens + (int) ceil(numeroItens * fatorDeCarga);
    cout << "Fator de carga: " << setprecision(2) << 1.0 - fatorDeCarga << endl;
    cout << "Tamanho real do vetor: " << tamanhoVetor << endl;
    return tamanhoVetor;
}

unsigned long calculaTamanhoVetorUsandoProxNumeroPrimo(unsigned long numeroItens, unsigned long tamanhoVetor) {
    unsigned long novoTamanhoVetor = proximoPrimo(tamanhoVetor);
    cout << "Utilizando primo " << novoTamanhoVetor << " ao invés  de " << tamanhoVetor << endl;
    double fatorDeCargaReal = 1.0 - ((double) (novoTamanhoVetor - numeroItens) / numeroItens);
    cout << "Fator de carga real: " << fatorDeCargaReal << endl;
    return novoTamanhoVetor;
}

unsigned long *criaVetorTabelaHash(unsigned long tamanhoVetor) {
    unsigned long *tabelaHash = new unsigned long[tamanhoVetor];

    // Enche o vetor com VAZIO
    memset(tabelaHash, VALOR_FLAG_VAZIO, tamanhoVetor * sizeof(tabelaHash[0]));
    return tabelaHash;
}

void printEstatisticas(EstatisticasChave *estatisticasChaves,
                       EstatisticasEstrutura estatisticasEstrutura, unsigned long tamanhoVetor) {
    unsigned long totalColisoes = 0, quantidadeGerada = 0;
    for (unsigned long i = 0; i < tamanhoVetor; i++) {
        totalColisoes += estatisticasChaves[i].colisoes;
        quantidadeGerada += estatisticasChaves[i].quantidadeGerada;
    }

    cout << std::setprecision(10);
    cout << "Colisoes na estrutura: " << totalColisoes << endl;
    double mediaColisoes = ((double) totalColisoes / (double) tamanhoVetor) * 100;
    cout << "Media de colisoes: " << mediaColisoes << endl;

    cout << "Numero de comparacoes na estrutura: " << estatisticasEstrutura.comparacoes << endl;
    double mediaComparacoes = ((double) estatisticasEstrutura.comparacoes / (double) tamanhoVetor) * 100;
    cout << "Media de comparacoes: " << mediaComparacoes << endl;

    cout << "Numero de falhas de insercao na estrutura: " << estatisticasEstrutura.falhas << endl;
//    cout << "Quantidade de chaves geradas: " << quantidadeGerada << endl;
    cout << "Numero de chaves repetidas: " << estatisticasEstrutura.repetidos << endl;
}

EstatisticasChave *criaVetorEstatisticasChave(unsigned long tamanhoVetor) {
    EstatisticasChave *estatisticasChaves = new EstatisticasChave[tamanhoVetor];

    for (unsigned long i = 0; i < tamanhoVetor; i++) {
        EstatisticasChave chave;
        estatisticasChaves[i] = chave;
    }
    return estatisticasChaves;
}

EstatisticasEstrutura criaEstatisticasEstrutura() {
    EstatisticasEstrutura estatisticasEstrutura;
    return estatisticasEstrutura;
}

void insercaoLinear(unsigned long *vetorNumerosAleatorios, unsigned long numeroItens, float fatorDeCarga,
                    bool usarProxPrimo) {
    cout << "****** Insercao Linear ******" << endl;

    unsigned long tamanhoVetor = calculaTamanhoVetor(numeroItens, fatorDeCarga);

    if (usarProxPrimo) {
        tamanhoVetor = calculaTamanhoVetorUsandoProxNumeroPrimo(numeroItens, tamanhoVetor);
    }

    unsigned long *tabelaHash = criaVetorTabelaHash(tamanhoVetor);

    EstatisticasChave *estatisticasChaves = criaVetorEstatisticasChave(tamanhoVetor);
    EstatisticasEstrutura estatisticasEstrutura = criaEstatisticasEstrutura();

    double start, end;
    start = getCurrentTime();
    for (unsigned long j = 0; j < numeroItens; j++) {
        hashInsereLinear(vetorNumerosAleatorios[j], tabelaHash, tamanhoVetor,
                         estatisticasChaves, estatisticasEstrutura);
    }
    end = getCurrentTime();

    delete[] tabelaHash;

    // Estatisticas
    printTimeDiff(start, end);
    printEstatisticas(estatisticasChaves, estatisticasEstrutura, tamanhoVetor);

    cout << "***********************" << endl;
}

void hashInsereLinear(int elemento, unsigned long *T, unsigned long tamanhoVetorHash,
                      EstatisticasChave *estatisticasChaves, EstatisticasEstrutura &estatisticasEstrutura) {
    int chave = calculaHash(abs(elemento), tamanhoVetorHash);
    estatisticasChaves[chave].quantidadeGerada++;

    int posicao = chave;
    for (unsigned long i = 1; i <= tamanhoVetorHash; i++) {
        estatisticasEstrutura.comparacoes++;
        if (T[posicao] == VALOR_FLAG_VAZIO) {
            T[posicao] = elemento;
            //cout<<"Insercao:: posicao="<<posicao<<", elemento:"<<elemento<<endl;
            return;
        } else if (T[posicao] == elemento) {
            //cout << "Elemento repetido: " << elemento << ", não será inserido." << endl;
            estatisticasEstrutura.repetidos++;
            return;
        }

        // Ocorreu colisao
        estatisticasChaves[posicao].colisoes++;
        // Percorre circular
        posicao = (chave + i) % tamanhoVetorHash;
    }

    //Ocorreu algum erro na inserção
    estatisticasEstrutura.falhas++;
}

void insercaoQuadratica(unsigned long *vetorNumerosAleatorios, unsigned long numeroItens, float fatorDeCarga,
                        bool usarProxPrimo) {
    cout << "****** Inserção Quadratica ******" << endl;

    unsigned long tamanhoVetor = calculaTamanhoVetor(numeroItens, fatorDeCarga);

    if (usarProxPrimo) {
        tamanhoVetor = calculaTamanhoVetorUsandoProxNumeroPrimo(numeroItens, tamanhoVetor);
    }

    unsigned long *tabelaHash = criaVetorTabelaHash(tamanhoVetor);

    EstatisticasChave *estatisticasChaves = criaVetorEstatisticasChave(tamanhoVetor);
    EstatisticasEstrutura estatisticasEstrutura = criaEstatisticasEstrutura();

    double start, end;
    start = getCurrentTime();
    for (unsigned long j = 0; j < numeroItens; j++) {
        hashInsereQuadratica(vetorNumerosAleatorios[j], tabelaHash, tamanhoVetor,
                             estatisticasChaves, estatisticasEstrutura);
    }
    end = getCurrentTime();

    delete[] tabelaHash;

    printTimeDiff(start, end);
    printEstatisticas(estatisticasChaves, estatisticasEstrutura, tamanhoVetor);

    cout << "***********************" << endl;
}


void hashInsereQuadratica(int elemento, unsigned long *T, unsigned long tamanhoVetorHash,
                          EstatisticasChave *estatisticasChaves, EstatisticasEstrutura &estatisticasEstrutura) {
    int chave = calculaHash(abs(elemento), tamanhoVetorHash);

    estatisticasChaves[chave].quantidadeGerada++;
    int posicao = chave;
    unsigned int somaQuadratica = 0;
    unsigned int incrementa = 1;
    while (true) {
        estatisticasEstrutura.comparacoes++;
        if (somaQuadratica != 0 && posicao == chave) {
            // Código deu uma volta
            estatisticasEstrutura.falhas++;
            return;
        }
        if (T[posicao] == VALOR_FLAG_VAZIO) {
            T[posicao] = elemento;
            //cout<<"Insercao:: posicao="<<posicao<<", elemento:"<<elemento<<endl;
            return;
        } else if (T[posicao] == elemento) {
            estatisticasEstrutura.repetidos++;
            //cout << "Elemento repetido: " << elemento << ", não será inserido." << endl;
            return;
        } else {
            somaQuadratica = incrementa * incrementa;
            //cout<<"Colisao: "<<somaQuadratica<<", i: "<<incrementa<<endl;
            incrementa++;
        }

        // Ocorreu colisão
        estatisticasChaves[posicao].colisoes++;

        // Percorre circular
        posicao = (chave + somaQuadratica) % tamanhoVetorHash;
    }
}

int calculaHash(int chave, int tamanho) {
    return (chave % tamanho);
}

int proximoPrimo(int n) {
    while (!numeroEprimo(++n)) {}
    return n;
}

bool numeroEprimo(int n) {
    for (int i = 2; i < n; i++) {
        if (n % i == 0 && i != n) return false;
    }
    return true;
}

/* Funções de RNG */

unsigned long *populateArrayWithRandomNumbers(unsigned long size) {
    unsigned long *array = new unsigned long[size];

    double start = getCurrentTime();

    for (unsigned long i = 0; i < size; i++) {
        int r = geraNumeroRandomico();
        array[i] = r;
    }

    double end = getCurrentTime();
    printTimeDiff(start, end);

    return array;
}

void printTimeDiff(double start, double end) {
    if (std::isnan(start) || std::isnan(end)) {
        return;
    }
    double diff = end - start;
    double microsecondsTotal = diff;
    double secondsTotal = diff * chrono::microseconds::period::num /
                          chrono::microseconds::period::den;
    double millisecondsTotal = diff * chrono::milliseconds::period::num /
                               chrono::milliseconds::period::den;

    cout << "Execucao: " << setprecision(10);
    //cout << microsecondsTotal << "μs, ";
    cout << millisecondsTotal << "ms, ";
    //cout << secondsTotal << "s";
    cout << endl;
}

