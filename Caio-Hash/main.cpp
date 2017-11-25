#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <Windows.h>

using namespace std;
int A[0];
int B[0];

int calculaHash(int elemento);
int randomGen();
int main()
{
    int n;
    cout<<"Insira o de n:"<<endl;
    cin>>n;

    for(int i=0; i<n; i++)
    {
        cout<<"i: "<<i<<endl;
        cout<<"random: "<<randomGen()<<endl;
    }

    clock_t start = clock();

    for(int i=0; i<100000000; i++)
    {

    }

    clock_t end = clock();

    cout << "Execucao em millisegundos: " << (end-start)/double(CLOCKS_PER_SEC)*1000 << endl;

    return 0;
}


void Hash_Insere_Linear(int elemento, int T[])
{
    int posicao = calculaHash(elemento);
    do
    {
        // Permite inserção de repetido
        if (T[posicao] == NULL)
        {
            T[posicao] = elemento;
            return;
        }
        else
        {
            posicao++;
        }
    }
    while(posicao < 1000);
}

int randomGen()
{
    Sleep(1);
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);
    return distribution(generator);
}
