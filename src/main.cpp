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

int tsize = 11;

/* Definições */

struct estatisticas_chave {
    long colisoes;
    bool ocupada;
    int chave;
};

int *populateArrayWithRandomNumbers(int size);

void printTimeDiff(double start, double end);

void linear_insert();

bool numeroEprimo(int n);

int numeroPrimoAnterior(int n);

int proximoPrimo(int n);

int geraNumeroRandomico();

void hashInsereLinear(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo, estatisticas_chave *estatisticasChaves);

void hashInsereQuadratica(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo = 0);

int calculaHash(int chave, int tamanho);

void insercaoLinear(int *vetorNumerosAleatorios, int numeroItens, float fatorDeCarga);

void insercaoQuadratica(int *vetorNumerosAleatorios, int numeroItens, float fatorDeCarga);

const int VALOR_FLAG_VAZIO = -1;
const float LIMITE_FATOR_DE_CARGA = 0.7;

/* Main */
int main(int argc, char *argv[]) {
    int n = 1000;
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <tamanho do n>" << endl;

//        cout << "Insira o valor de n:" << endl;
//        cin >> n;
    } else {
        n = stoi(argv[1]);
    }

    cout << "Gerando " << n << " números aleatórios entre [" << RNG_MIN << "] e [" << RNG_MAX << "]" << endl;

    int *vetorNumerosAleatorios = populateArrayWithRandomNumbers(n);

    cout << "Números aleatórios gerados." << endl;

    for (float fatorDeCarga = 0.1; fatorDeCarga <= LIMITE_FATOR_DE_CARGA; fatorDeCarga += 0.1) {
        insercaoLinear(vetorNumerosAleatorios, n, fatorDeCarga);
        insercaoQuadratica(vetorNumerosAleatorios, n, fatorDeCarga);
    }

    return 0;
}

int calculaTamanhoVetor(int numeroItens, float fatorDeCarga) {
    int tamanhoVetor = numeroItens + (int) ceil(numeroItens * fatorDeCarga);
    cout << "Fator de carga: " << setprecision(2) << 1.0 - fatorDeCarga << endl;
    cout << "Tamanho real do vetor: " << tamanhoVetor << endl;
    return tamanhoVetor;
}

int *criaVetorTabelaHash(int tamanhoVetor) {
    int *tabelaHash = new int[tamanhoVetor];

    // Enche o vetor com VAZIO
    memset(tabelaHash, VALOR_FLAG_VAZIO, tamanhoVetor * sizeof(tabelaHash[0]));
    return tabelaHash;
}

void insercaoLinear(int *vetorNumerosAleatorios, int numeroItens, float fatorDeCarga) {
    cout << "****** Inserção Linear ******" << endl;

    int tamanhoVetor = calculaTamanhoVetor(numeroItens, fatorDeCarga);
    int *tabelaHash = criaVetorTabelaHash(tamanhoVetor);

    estatisticas_chave estatisticasChaves[tamanhoVetor];
    for (int i = 0; i < tamanhoVetor; ++i) {
        estatisticas_chave chave;
        chave.colisoes = 0;
        estatisticasChaves[i] = chave;
    }

    double start, end;
    start = getCurrentTimeInMillis();
    for (int j = 0; j < numeroItens; j++) {
        hashInsereLinear(vetorNumerosAleatorios[j], tabelaHash, tamanhoVetor, 0, estatisticasChaves);
    }
    end = getCurrentTimeInMillis();
    cout << "Tempos de inserção: " << endl;

    delete[] tabelaHash;

    // Estatísticas
    printTimeDiff(start, end);

    long totalColisoes = 0;
    for (int i = 0; i < tamanhoVetor; ++i) {
        totalColisoes += estatisticasChaves[i].colisoes;
    }
    cout << "Colisões na estrutura: " << totalColisoes << endl;

    cout << "***********************" << endl;
}

void insercaoQuadratica(int *vetorNumerosAleatorios, int numeroItens, float fatorDeCarga) {
    cout << "****** Inserção Quadrática ******" << endl;

    int tamanhoVetor = calculaTamanhoVetor(numeroItens, fatorDeCarga);
    int *tabelaHash = criaVetorTabelaHash(tamanhoVetor);

    double start, end;
    start = getCurrentTimeInMillis();
    for (int j = 0; j < numeroItens; j++) {
        hashInsereQuadratica(vetorNumerosAleatorios[j], tabelaHash, tamanhoVetor);
    }
    end = getCurrentTimeInMillis();
    cout << "Tempos de inserção: " << endl;

    delete[] tabelaHash;

    printTimeDiff(start, end);
    //TODO: print statistics

    cout << "***********************" << endl;
}

/* Funções do Caio-Hash */

void hashInsereLinear(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo, estatisticas_chave estatisticasChaves[]) {
    int chave = calculaHash(abs(elemento), tamanhoVetorHash);
    if (numeroPrimo != 0) {
        int chave = calculaHash(elemento, numeroPrimo);
    }
    estatisticasChaves[chave].colisoes++;

    int posicao = chave;
    for (int i = 1; i <= tamanhoVetorHash; i++) {
        if (T[posicao] == VALOR_FLAG_VAZIO) {
            T[posicao] = elemento;
            //cout<<"Insercao:: posicao="<<posicao<<", elemento:"<<elemento<<endl;
            return;
        } else if (T[posicao] == elemento) {
            //cout << "Elemento repetido: " << elemento << ", não será inserido." << endl;
            return;
        }

        // Ocorreu colisão
        estatisticasChaves[posicao].colisoes++;
        // Percorre circular
        posicao = (chave + i) % tamanhoVetorHash;
    }

    cout << "Percorrido n posicoes insercao invalida!" << endl;
}

void hashInsereQuadratica(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo) {
    int chave = calculaHash(abs(elemento), tamanhoVetorHash);
    if (numeroPrimo != 0) {
        int chave = calculaHash(elemento, numeroPrimo);
    }

    int posicao = chave;
    unsigned int somaQuadratica = 0;
    unsigned int incrementa = 1;
    while (true) {
        if (somaQuadratica != 0 && posicao == chave) {
            cout << "Deu uma volta" << endl;
            return;
        }
        if (T[posicao] == VALOR_FLAG_VAZIO) {
            T[posicao] = elemento;
            //cout<<"Insercao:: posicao="<<posicao<<", elemento:"<<elemento<<endl;
            return;
        } else if (T[posicao] == elemento) {
            //cout << "Elemento repetido: " << elemento << ", não será inserido." << endl;
            return;
        } else {
            somaQuadratica = incrementa * incrementa;
            //cout<<"Colisao: "<<somaQuadratica<<", i: "<<incrementa<<endl;
            incrementa++;
        }

        // Percorre circular
        posicao = (chave + somaQuadratica) % tamanhoVetorHash;
    }
}

int calculaHash(int chave, int tamanho) {
    return (chave % tamanho);
}

int numeroPrimoAnterior(int n) {
    if (n < 2) {
        return 1;
    }

    while (!numeroEprimo(--n)) {}
    return n;
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

int *populateArrayWithRandomNumbers(int size) {
    int *array = new int[size];

    double start = getCurrentTimeInMillis();

    for (int i = 0; i < size; i++) {
        int r = geraNumeroRandomico();
        array[i] = r;
    }

    double end = getCurrentTimeInMillis();
    printTimeDiff(start, end);

    return array;
}

void printTimeDiff(double start, double end) {
    if (std::isnan(start) || std::isnan(end)) {
        return;
    }
    double diff = end - start;
    double millisecondsTotal = diff;
    double secondsTotal = diff * chrono::milliseconds::period::num /
                          chrono::milliseconds::period::den;

    cout << "Execução: " << millisecondsTotal << "ms, " << secondsTotal << "s" << endl;
}

/* HASHING - LINEAR AND QUADRATIC PROBING */

int myHash(int key) {
    int i;
    i = key % tsize;
    return i;
}

//-------Sondagem Linear-------
int linear_hash(int key) {
    int i;
    i = (key + 1) % tsize;
    return i;
}

//-------Sondagem Quadrática-------
int squad_hash(int key, int j) {
    int i;
    i = (key + (j * j)) % tsize;
    return i;
}

void linear_insert() {
    int hash_array[11];
    int hash_value;
    int elem_array[] = {7, 36, 18, 62};
    int key;

    for (int l = 0; l < 11; l++) {
        hash_array[l] = -1;
    }
    //exemplo 1 n = m
    //total de elementos n = 7
    int n = 4;
    for (int k = 0; k < n; k++) {
        key = elem_array[k];
        hash_value = myHash(key);
        while (hash_array[hash_value] != -1) {
            hash_value = linear_hash(hash_value);

        }
        hash_array[hash_value] = key;
    }
    cout << "Mostrando os elementos do hash_array" << endl;
    for (int i = 0; i < 11; i++) {
        cout << "Elemento na posição " << i << ": " << hash_array[i] << endl;
        //printf("\n  Element at position %d: %d",i,hash_array[i]);
    }
}

void squad_insert() {
    int hash_array[7];
    int hash_value;
    int elem_array[] = {50, 700, 76, 85, 92, 73, 101};
    int key, j;

    for (int l = 0; l < 7; l++) {
        hash_array[l] = -1;
    }
    //exemplo 1 n = m
    //total de elementos n = 7
    int n = 7;
    for (int k = 0; k < n; k++) {
        j = 1;
        key = elem_array[k];
        hash_value = myHash(key);
        while (hash_array[hash_value] != -1) {
            hash_value = squad_hash(hash_value, j);
            j++;
        }
        hash_array[hash_value] = key;
    }
    cout << "Mostrando os elementos do hash_array" << endl;
    for (int i = 0; i < 7; i++) {
        cout << "Elemento na posição " << i << ": " << hash_array[i] << endl;
        //printf("\n  Element at position %d: %d",i,hash_array[i]);
    }
}

