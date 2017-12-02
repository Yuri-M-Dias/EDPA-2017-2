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

typedef struct EstatisticasChave {
    long colisoes;
    long quantidadeGerada;
};

typedef struct EstatisticasEstrutura {
    long repetidos;
};

int *populateArrayWithRandomNumbers(int size);

void printTimeDiff(double start, double end);

void linear_insert();

bool numeroEprimo(int n);

int numeroPrimoAnterior(int n);

int proximoPrimo(int n);

int geraNumeroRandomico();

void hashInsereLinear(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo,
                      EstatisticasChave estatisticasChaves[], EstatisticasEstrutura &estatisticasEstrutura);

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

    EstatisticasChave estatisticasChaves[tamanhoVetor];
    for (int i = 0; i < tamanhoVetor; ++i) {
        EstatisticasChave chave;
        chave.colisoes = 0;
        chave.quantidadeGerada = 0;
        estatisticasChaves[i] = chave;
    }
    EstatisticasEstrutura estatisticasEstrutura;
    estatisticasEstrutura.repetidos = 0;

    double start, end;
    start = getCurrentTimeInMillis();
    for (int j = 0; j < numeroItens; j++) {
        hashInsereLinear(vetorNumerosAleatorios[j], tabelaHash, tamanhoVetor, 0,
                         estatisticasChaves, estatisticasEstrutura);
    }
    end = getCurrentTimeInMillis();

    delete[] tabelaHash;

    // Estatísticas
    printTimeDiff(start, end);

    long totalColisoes = 0, quantidadeGerada = 0;
    for (int i = 0; i < tamanhoVetor; ++i) {
        totalColisoes += estatisticasChaves[i].colisoes;
        quantidadeGerada += estatisticasChaves[i].quantidadeGerada;
    }
    cout << "Colisões na estrutura: " << totalColisoes << endl;
    cout << "Quantidade de chaves geradas: " << quantidadeGerada << endl;
    cout << "Número de chaves repetidas: " << estatisticasEstrutura.repetidos << endl;

    cout << "***********************" << endl;
}

void hashInsereLinear(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo,
                      EstatisticasChave estatisticasChaves[], EstatisticasEstrutura &estatisticasEstrutura) {
    int chave = calculaHash(abs(elemento), tamanhoVetorHash);
    if (numeroPrimo != 0) {
        int chave = calculaHash(elemento, numeroPrimo);
    }
    estatisticasChaves[chave].quantidadeGerada++;

    int posicao = chave;
    for (int i = 1; i <= tamanhoVetorHash; i++) {
        if (T[posicao] == VALOR_FLAG_VAZIO) {
            T[posicao] = elemento;
            //cout<<"Insercao:: posicao="<<posicao<<", elemento:"<<elemento<<endl;
            return;
        } else if (T[posicao] == elemento) {
            //cout << "Elemento repetido: " << elemento << ", não será inserido." << endl;
            estatisticasEstrutura.repetidos++;
            return;
        }

        // Ocorreu colisão
        estatisticasChaves[posicao].colisoes++;
        // Percorre circular
        posicao = (chave + i) % tamanhoVetorHash;
    }

    cout << "Percorrido n posicoes insercao invalida!" << endl;
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

int tamanhoArray(float fatorCarga, int n){
    float j = (fatorCarga - 1) * (-1);
    float x = n*(j*n);
    int resul = ceil(x);

    if (resul < n)
        return n + resul;
    return  resul;
}


int hashFunction(int key, int tsize) {
    int i;
    i = key % tsize;
    return i;
}

//-------Sondagem Linear-------
int hashLinear(int key, int tsize) {
    int i;
    i = (key + 1) % tsize;
    return i;
}

//-------Sondagem Quadrática-------
int hashQuadratico(int key, int j, int tsize) {
    int i;
    i = (key + (j * j)) % tsize;
    return i;
}


void insertLinear() {
    //defifindo o valor de hash
    int hashValue;
    //criando um vetor com valores conhecidos
    std::vector<int> elem_array {7, 36, 18, 62};
    int key;

    //definindo os valores de cargas de exemplo
    float fatorCarga_1 = 0.1;
    //float fatorCarga_2 = 0.5;
    float fatorCarga_2 = 0.36;
    float fatorCarga_3 = 0.7;
    float fatorCarga_4 = 0.9;
    //Recebe o tamanho do novo vetor mediante o fator de carga escolhido
    int sizeArray = tamanhoArray(fatorCarga_2, elem_array.size());

    cout << "Tamanho do Vetor mediante Fator de Carga = "  << fatorCarga_2 <<  " : " << sizeArray << endl;
    //criando o vetor de hash com o tamanho informado
    int hashArray[sizeArray];
    //preencendo os valore do vetor com -1 o que indica que ele está vazio
    for (int l = 0; l < sizeArray; l++) {
        hashArray[l] = -1;
    }
    //contador de consisões
    int colision = 0;
    int n = elem_array.size();
    for (int k = 0; k < n; k++) {
        key = elem_array[k];
        hashValue = hashFunction(key, sizeArray);
        while (hashArray[hashValue] != -1) {
            hashValue = hashLinear(hashValue, sizeArray);
        }
        hashArray[hashValue] = key;
        //Contadtor de colisões
        if(hashArray[k] != -1){
            colision++;
        }
    }
    cout << "Mostrando os elementos do hashArray" << endl;
    for (int i = 0; i < sizeArray; i++) {
        cout << "Elemento na posição " << i << ": " << hashArray[i] << endl;
    }

    float mediaColisoes = (float)colision/(float)elem_array.size();
    cout << "\nColisões encontradas " << colision << endl;
    cout << "\nColisões em relação a N = " << mediaColisoes * 100 << "%"<< endl;
}

void insertQuadratico() {
    int hashValue;
    std::vector<int> elem_array {7, 36, 18, 62};
    int key;

    float fatorCarga_1 = 0.1;
    //float fatorCarga_2 = 0.5;
    float fatorCarga_2 = 0.36;
    float fatorCarga_3 = 0.7;
    float fatorCarga_4 = 0.9;

    int sizeArray = tamanhoArray(fatorCarga_2, elem_array.size());

    cout << "Tamanho do Vetor mediante Fator de Carga = "  << fatorCarga_2 <<  " : " << sizeArray << endl;

    int hashArray[sizeArray];

    for (int l = 0; l < sizeArray; l++) {
        hashArray[l] = -1;
    }

    int n = elem_array.size();
    int colision = 0;
    int j = 0;
    for (int k = 0; k < n; k++) {
        j = 1;
        key = elem_array[k];
        hashValue = hashFunction(key, sizeArray);
        while (hashArray[hashValue] != -1) {
            hashValue = hashQuadratico(hashValue, j, sizeArray);
            j++;
        }
        hashArray[hashValue] = key;
        //Contadtor de colisões
        if(hashArray[k] != -1){
            colision++;
        }
    }
    cout << "Mostrando os elementos do hash_array" << endl;
    for (int i = 0; i < sizeArray; i++) {
        cout << "Elemento na posição " << i << ": " << hashArray[i] << endl;
    }

    float mediaColisoes = (float)colision/(float)elem_array.size();
    cout << "\nColisões encontradas " << colision << endl;
    cout << "\nColisões em relação a N = " << mediaColisoes * 100 << "%"<< endl;
}

int main() {

    insertLinear();
    //insertQuadratico();

    return 0;
}