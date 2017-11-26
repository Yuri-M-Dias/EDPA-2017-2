

#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <climits>

#define INTERACTIVE 1
#define HASH_SIZE 7

typedef struct
{
    int key[HASH_SIZE];
    int state[HASH_SIZE];
    /* -1=lazy delete, 0=empty, 1=occupied */
} hashtable;

using namespace std;
int myHash(int input);

int generateRandomNumber() {
    this_thread::sleep_for(chrono::milliseconds(1));
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);

    return distribution(generator);
}

int hash_fun(int input)
{
    return input % HASH_SIZE;
}

int linear(int i, int input)
{
    return (hash_fun(input) + i) % HASH_SIZE;
}

int quad(int i, int input)
{
    return (hash_fun(input) + i * i) % HASH_SIZE;
}

void open_addressing_insert(int item, hashtable * ht )
{
    int hash_value;
    hash_value = myHash(item);
    int i = hash_value;
    while (ht->state[i]== 1)
    {
        if (ht->key[i] == item) {
            fprintf(stderr,"Duplicate entry\n");
            exit(1);
        }
        //i = (i + F(i)) % HASH_SIZE;
        i = linear(i,i);
        if (i == hash_value) {
            fprintf(stderr, "The table is full\n");
            exit(1);
        }
    }
    ht->key[i] = item;
    ht->state[i] = 1;
}

//int main() {
//    int n;
//    int vetor[] = {50, 700, 76, 85, 92, 73, 101};
//    int vetorB[7];
//
//    for (int j = 0; j < 6; ++j) {
//
//    }
//
//#if INTERACTIVE
//    cout << "Insira o valor de n:" << endl;
//    cin >> n;
//#else
//    n = 500;
//#endif
//
//    cout << "Tomando n como " << n << endl;
//
//    clock_t start = clock();
//
//    for (int i = 0; i < n; i++) {
//        int r = generateRandomNumber();
//    }
//
//    clock_t end = clock();
//
//    // CLOCKS_PER_SEC == 10^6 = 1 microsecond
//    double microsecondsTotal = difftime(end, start);
//    double millisecondsTotal = microsecondsTotal / 1000;
//    double secondsTotal = microsecondsTotal / double(CLOCKS_PER_SEC);
//
//    cout << "Execucao em microsegundos: " << microsecondsTotal << endl;
//    cout << "Execucao em millisegundos: " << millisecondsTotal << endl;
//    cout << "Execucao em segundos: " << secondsTotal << endl;
//
//    return 0;
//}
//
