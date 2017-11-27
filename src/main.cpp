//
// Created by wgalvao on 11/26/17.
//

/* HASHING - LINEAR AND QUADRATIC PROBING */

#include <stdio.h>
#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <climits>

// Usando para não repetir INT_MAX e INT_MIN em todo lugar
#define RNG_MAX INT16_MAX
#define RNG_MIN INT16_MIN

#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

#define INTERACTIVE false

using namespace std;
int tsize = 11;

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
    if(std::isnan(start) || std::isnan(end)){
        return;
    }
    double diff = end - start;
    double millisecondsTotal = diff;
    double secondsTotal = diff * chrono::milliseconds::period::num /
                          chrono::milliseconds::period::den;

    cout << "Execução em millisegundos: " << millisecondsTotal << endl;
    cout << "Execução em segundos: " << secondsTotal << endl;
}

int myHash(int key)
{
    int i ;
    i = key % tsize ;
    return i;
}

//-------Sondagem Linear-------
int linear_hash(int key)
{
    int i ;
    i = (key + 1) % tsize ;
    return i ;
}

//-------Sondagem Quadrática-------
int squad_hash(int key, int j)
{
    int i ;
    i = (key + (j * j)) % tsize ;
    return i ;
}

void linear_insert(){
    int hash_array[11];
    int hash_value;
    int elem_array[] = {7, 36, 18, 62};
    int key;

    for (int l = 0; l < 11; l++) {
        hash_array[l] = -1;
    }
    //exemplo 1 n = m
    //total de elementos n = 7
    int n = 11;
    for(int k = 0; k < n; k++)
    {
        key = elem_array[k] ;
        hash_value = myHash(key);
        while (hash_array[hash_value] != -1)
        {
            hash_value = linear_hash(hash_value);

        }
        hash_array[hash_value] = key ;
    }
    cout << "Mostrando os elementos do hash_array" << endl;
    for (int i = 0; i < 11; i++)
    {
        cout << "Elemento na posição " << i << ": " << hash_array[i] << endl;
        //printf("\n  Element at position %d: %d",i,hash_array[i]);
    }
}

void squad_insert(){
    int hash_array[7];
    int hash_value;
    int elem_array[] = {50, 700, 76, 85, 92, 73, 101};
    int key ,j;

    for (int l = 0; l < 7; l++) {
        hash_array[l] = -1;
    }
    //exemplo 1 n = m
    //total de elementos n = 7
    int n = 7;
    for(int k = 0; k < n; k++)
    {
        j = 1;
        key = elem_array[k] ;
        hash_value = myHash(key);
        while (hash_array[hash_value] != -1)
        {
            hash_value = squad_hash(hash_value,j);
            j++;
        }
        hash_array[hash_value] = key ;
    }
    cout << "Mostrando os elementos do hash_array" << endl;
    for (int i = 0; i < 7; i++)
    {
        cout << "Elemento na posição " << i << ": " << hash_array[i] << endl;
        //printf("\n  Element at position %d: %d",i,hash_array[i]);
    }
}

int main() {
    int n;
#if INTERACTIVE
    cout << "Insira o valor de n:" << endl;
    cin >> n;
#else
    n = 5000;
#endif
    cout << "n = " << n << endl;
    cout << "Gerando números aleatórios entre [" << RNG_MIN << "] e [" << RNG_MAX << "]" << endl;

    int *array = populateArrayWithRandomNumbers(n);

//    for (int i = 0; i < n; ++i) {
//        cout << "A["<< i <<"] = " << array[i] << endl;
//    }

    return 0;
}