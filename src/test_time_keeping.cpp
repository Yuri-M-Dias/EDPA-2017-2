#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <climits>

using namespace std;

// Esses não deveriam estar em um .h?
int calculaHash(int chave, int tamanho);

int geraNumeroRandomico();

void hash_Insere_Linear(int elemento, int T[], int tamanho);

int main() {
    int n;
    cout << "Insira o valor de n:" << endl;
    cin >> n;

    int A[n];
    clock_t start = clock();

    for (int i = 0; i < n; i++) {
        int r = geraNumeroRandomico();
        A[i] = r;
    }

    clock_t end = clock();

    // CLOCKS_PER_SEC == 10^6 = 1 microsecond
    double microsecondsTotal = difftime(end, start);
    double millisecondsTotal = microsecondsTotal / 1000;
    double secondsTotal = microsecondsTotal / double(CLOCKS_PER_SEC);

    cout << "Execucao em microsegundos: " << microsecondsTotal << endl;
    cout << "Execucao em millisegundos: " << millisecondsTotal << endl;
    cout << "Execucao em segundos: " << secondsTotal << endl;

    return 0;
}

void hash_Insere_Linear(int elemento, int T[], int tamanho) {
    int posicao = calculaHash(elemento, tamanho);

    do {
        // Permite inserção de repetido
        if (T[posicao] == NULL) {
            T[posicao] = elemento;
            return;
        } else {
            posicao++;
        }
    } while (posicao < 1000);
}

int geraNumeroRandomico() {
    this_thread::sleep_for(chrono::milliseconds(1));
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);

    return distribution(generator);
}

int calculaHash(int chave, int tamanho) {
    return chave % tamanho;
}
