#include <iostream>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include <climits>

using namespace std;

int calculaHash(int chave, int tamanho);
int geraNumeroRandomico();
int hash_Insere_Linear();
int main()
{
    int n;
    cout<<"Insira o valor de n:"<<endl;
    cin>>n;

    int A[n];
    int B[n];

    clock_t start = clock();

    for(int i=0; i<n; i++)
    {
        //cout<<"i: "<<i<<endl;
        int r = geraNumeroRandomico();
        //cout<<"random: "<<geraNumeroRandomico()<<endl;
        A[i]=r;
        //B[i]=r;
    }
    clock_t end = clock();


    /*for(int i=0; i<n; i++)
    {
      //  hash_Insere_Linear()
    }
    */


    cout << "execucao em millisegundos: " << (end-start)*1000/double(CLOCKS_PER_SEC) << endl;

    return 0;
}

void hash_Insere_Linear(int elemento, int T[], int tamanho)
{
    int posicao = calculaHash(elemento, tamanho);

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

int geraNumeroRandomico()
{
    this_thread::sleep_for(chrono::milliseconds(1));
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator (seed);
    uniform_int_distribution<int> distribution(INT_MIN, INT_MAX);

    return distribution(generator);
}

int calculaHash(int chave, int tamanho)
{
    return chave % tamanho;
}
