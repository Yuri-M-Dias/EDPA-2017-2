#include <cstdio>
#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>

// Necessário apenas se usar INT_MAX e INT_MIN do C(32-bits precision)
#include <climits>

// Usando para não repetir INT_MAX e INT_MIN em todo lugar
#define RNG_MAX INT16_MAX
#define RNG_MIN INT16_MIN

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

#define INTERACTIVE false

using namespace std;

/* Definições */

int geraNumeroRandomico();

int *populateArrayWithRandomNumbers(int size);

void printTimeDiff(double start, double end);

double getCurrentTimeInMillis();

/* Funções */

int geraNumeroRandomico() {
    this_thread::sleep_for(chrono::milliseconds(1));
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(RNG_MIN, RNG_MIN);
    return distribution(generator);
}

double getCurrentTimeInMillis() {
    return chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now().time_since_epoch()
    ).count();
}

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

    cout << "Execução em millisegundos: " << millisecondsTotal << endl;
    cout << "Execução em segundos: " << secondsTotal << endl;
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