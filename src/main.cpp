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


