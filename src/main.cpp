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
    EstatisticasChave() {
        this->quantidadeGerada = 0;
        this->colisoes = 0;
    };
    unsigned long colisoes;
    unsigned long quantidadeGerada;
};

typedef struct EstatisticasEstrutura {
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

void linear_insert();

bool numeroEprimo(int n);

int numeroPrimoAnterior(int n);

int proximoPrimo(int n);

int geraNumeroRandomico();

void hashInsereLinear(int elemento, unsigned long *T, unsigned long tamanhoVetorHash, int numeroPrimo,
                      EstatisticasChave *estatisticasChaves, EstatisticasEstrutura &estatisticasEstrutura);

void hashInsereQuadratica(int elemento, unsigned long *T, unsigned long tamanhoVetorHash, int numeroPrimo,
                          EstatisticasChave *estatisticasChaves, EstatisticasEstrutura &estatisticasEstrutura);

int calculaHash(int chave, int tamanho);

void insercaoLinear(unsigned long *vetorNumerosAleatorios, unsigned long numeroItens, float fatorDeCarga,
                    bool usarProxPrimo = false);

void insercaoQuadratica(unsigned long *vetorNumerosAleatorios, unsigned long numeroItens, float fatorDeCarga,
                        bool usarProxPrimo = false);

void insertLinear();

/* Main */

const int VALOR_FLAG_VAZIO = -1;
const float LIMITE_FATOR_DE_CARGA = 1;

int main(int argc, char *argv[]) {
    unsigned long n = 100000;
    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <tamanho do n>" << endl;
        cerr << "Usando n com valor padrão: " << n << "" << endl;
    } else {
        n = stoi(argv[1]);
    }

    cout << "Gerando " << n << " números aleatórios entre [" << RNG_MIN << "] e [" << RNG_MAX << "]" << endl;

    unsigned long *vetorNumerosAleatorios = populateArrayWithRandomNumbers(n);

    cout << "Números aleatórios gerados." << endl;

    // Do Wellington
    //insertLinear();

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

    cout << std::setprecision(4);
    cout << "Colisões na estrutura: " << totalColisoes << endl;
    cout << "Quantidade de chaves geradas: " << quantidadeGerada << endl;
    cout << "Número de chaves repetidas: " << estatisticasEstrutura.repetidos << endl;
    cout << "Número de comparações na estrutura: " << estatisticasEstrutura.comparacoes << endl;
    cout << "Número de falhas de inserção na estrutura: " << estatisticasEstrutura.falhas << endl;
    double mediaColisoes = ((double) totalColisoes / (double) tamanhoVetor) * 100;
    cout << "Média de colisões: " << mediaColisoes << endl;

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
    cout << "****** Inserção Linear ******" << endl;

    unsigned long tamanhoVetor = calculaTamanhoVetor(numeroItens, fatorDeCarga);

    if (usarProxPrimo) {
        unsigned long numeroAnterior = tamanhoVetor;
        tamanhoVetor = proximoPrimo(tamanhoVetor);
        cout << "Utilizando primo " << tamanhoVetor << " ao invés " << numeroAnterior << endl;
        double fatorDeCargaReal = 1.0 - ((double) (tamanhoVetor - numeroItens) /  numeroItens);
        cout << "Fator de carga real: " << fatorDeCargaReal << endl;
    }

    unsigned long *tabelaHash = criaVetorTabelaHash(tamanhoVetor);

    EstatisticasChave *estatisticasChaves = criaVetorEstatisticasChave(tamanhoVetor);
    EstatisticasEstrutura estatisticasEstrutura = criaEstatisticasEstrutura();

    double start, end;
    start = getCurrentTime();
    for (unsigned long j = 0; j < numeroItens; j++) {
        hashInsereLinear(vetorNumerosAleatorios[j], tabelaHash, tamanhoVetor, 0,
                         estatisticasChaves, estatisticasEstrutura);
    }
    end = getCurrentTime();

    delete[] tabelaHash;

    // Estatísticas
    printTimeDiff(start, end);
    printEstatisticas(estatisticasChaves, estatisticasEstrutura, tamanhoVetor);

    cout << "***********************" << endl;
}

void hashInsereLinear(int elemento, unsigned long *T, unsigned long tamanhoVetorHash, int numeroPrimo,
                      EstatisticasChave *estatisticasChaves, EstatisticasEstrutura &estatisticasEstrutura) {
    int chave = calculaHash(abs(elemento), tamanhoVetorHash);
    if (numeroPrimo != 0) {
        int chave = calculaHash(elemento, numeroPrimo);
    }
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

        // Ocorreu colisão
        estatisticasChaves[posicao].colisoes++;
        // Percorre circular
        posicao = (chave + i) % tamanhoVetorHash;
    }

    //Ocorreu algum erro na inserção
    estatisticasEstrutura.falhas++;
}

void insercaoQuadratica(unsigned long *vetorNumerosAleatorios, unsigned long numeroItens, float fatorDeCarga,
                        bool usarProxPrimo) {
    cout << "****** Inserção Quadrática ******" << endl;

    unsigned long tamanhoVetor = calculaTamanhoVetor(numeroItens, fatorDeCarga);

    if (usarProxPrimo) {
        unsigned long numeroAnterior = tamanhoVetor;
        tamanhoVetor = proximoPrimo(tamanhoVetor);
        cout << "Utilizando primo " << tamanhoVetor << " ao invés " << numeroAnterior << endl;
        double fatorDeCargaReal = 1.0 - ((double) (tamanhoVetor - numeroItens) /  numeroItens);
        cout << "Fator de carga real: " << fatorDeCargaReal << endl;
    }

    unsigned long *tabelaHash = criaVetorTabelaHash(tamanhoVetor);

    EstatisticasChave *estatisticasChaves = criaVetorEstatisticasChave(tamanhoVetor);
    EstatisticasEstrutura estatisticasEstrutura = criaEstatisticasEstrutura();

    double start, end;
    start = getCurrentTime();
    for (unsigned long j = 0; j < numeroItens; j++) {
        hashInsereQuadratica(vetorNumerosAleatorios[j], tabelaHash, tamanhoVetor, 0,
                             estatisticasChaves, estatisticasEstrutura);
    }
    end = getCurrentTime();

    delete[] tabelaHash;

    printTimeDiff(start, end);
    printEstatisticas(estatisticasChaves, estatisticasEstrutura, tamanhoVetor);

    cout << "***********************" << endl;
}


void hashInsereQuadratica(int elemento, unsigned long *T, unsigned long tamanhoVetorHash, int numeroPrimo,
                          EstatisticasChave *estatisticasChaves, EstatisticasEstrutura &estatisticasEstrutura) {
    int chave = calculaHash(abs(elemento), tamanhoVetorHash);
    if (numeroPrimo != 0) {
        int chave = calculaHash(elemento, numeroPrimo);
    }

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

    cout << "Execução: " << setprecision(4);
    cout << microsecondsTotal << "μs, ";
    cout << millisecondsTotal << "ms, ";
    cout << secondsTotal << "s";
    cout << endl;
}

/* HASHING - LINEAR AND QUADRATIC PROBING */

int tamanhoArray(float fatorCarga, int n) {
    float j = (fatorCarga - 1) * (-1);
    float x = n * (j * n);
    int resul = ceil(x);

    if (resul < n)
        return n + resul;
    return resul;
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
    std::vector<int> elem_array{7, 36, 18, 62};
    int key;

    //definindo os valores de cargas de exemplo
    float fatorCarga_1 = 0.1;
    //float fatorCarga_2 = 0.5;
    float fatorCarga_2 = 0.36;
    float fatorCarga_3 = 0.7;
    float fatorCarga_4 = 0.9;
    //Recebe o tamanho do novo vetor mediante o fator de carga escolhido
    int sizeArray = tamanhoArray(fatorCarga_2, elem_array.size());

    cout << "Tamanho do Vetor mediante Fator de Carga = " << fatorCarga_2 << " : " << sizeArray << endl;
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
        if (hashArray[k] != -1) {
            colision++;
        }
    }
    cout << "Mostrando os elementos do hashArray" << endl;
    for (int i = 0; i < sizeArray; i++) {
        cout << "Elemento na posição " << i << ": " << hashArray[i] << endl;
    }

    float mediaColisoes = (float) colision / (float) elem_array.size();
    cout << "\nColisões encontradas " << colision << endl;
    cout << "\nColisões em relação a N = " << mediaColisoes * 100 << "%" << endl;
}

void insertQuadratico() {
    int hashValue;
    std::vector<int> elem_array{7, 36, 18, 62};
    int key;

    float fatorCarga_1 = 0.1;
    //float fatorCarga_2 = 0.5;
    float fatorCarga_2 = 0.36;
    float fatorCarga_3 = 0.7;
    float fatorCarga_4 = 0.9;

    int sizeArray = tamanhoArray(fatorCarga_2, elem_array.size());

    cout << "Tamanho do Vetor mediante Fator de Carga = " << fatorCarga_2 << " : " << sizeArray << endl;

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
        if (hashArray[k] != -1) {
            colision++;
        }
    }
    cout << "Mostrando os elementos do hash_array" << endl;
    for (int i = 0; i < sizeArray; i++) {
        cout << "Elemento na posição " << i << ": " << hashArray[i] << endl;
    }

    float mediaColisoes = (float) colision / (float) elem_array.size();
    cout << "\nColisões encontradas " << colision << endl;
    cout << "\nColisões em relação a N = " << mediaColisoes * 100 << "%" << endl;
}
