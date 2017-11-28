#include <iostream>
#include <iomanip>
#include <ctime>
#include <random>
#include <chrono>
#include <thread>
#include<limits>
#include<string.h>
using namespace std;

bool numeroEprimo(int n);
int numeroPrimoAnterior(int n);
int proximoPrimo(int n);
int geraNumeroRandomico();
void hashInsereLinear(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo=0);
void hashInsereQuadratica(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo=0);
int calculaHash(int chave, int tamanho);
void imprimeVetor(int V[], int tamanho);
double getCurrentTimeInMillis();
void printTimeDiff(double start, double end);
int geraNumeroRandomicoRand();

const int INVALID_ARRAY_VALUE=0;
const float LIMITE_FATOR_DE_CARGA = 0.1;
// Tratar para inserção de 0 numero aleatorio
// Erro na inserção primo proximo para valores rand menor que 0 o mod é negativo, tratar usando abs() gambiarra?

int main()
{
    int n=-1;
    while(n<1)
    {
         cout<<"Digite o tamanho do vetor n:"<<endl;
         cin>>n;

         if(n<1){
            cout<<"Valor invalido, n deve ser maior ou igual a 1"<<endl;
         }
    }

    int PROXIMO_PRIMO = proximoPrimo(n);
    int PRIMO_ANTERIOR = numeroPrimoAnterior(n);
    int vetorNumeroAleatorio[n];

    for(int i=0; i<n; i++)
    {
      vetorNumeroAleatorio[i] = geraNumeroRandomico();
    }

    cout<<"Finalizado geracao aleatoria"<<endl;

    //imprimeVetor(vetorNumeroAleatorio, n);

    for(float i=0.1; i<=LIMITE_FATOR_DE_CARGA; i+=0.1)
    {
        int tamanhoVetor = n + (int)ceil(n*i);
        int V_AUX[tamanhoVetor] = {0};
        memset(V_AUX, 0, sizeof(V_AUX));// Initializa com 0
        //imprimeVetor(V_AUX,tamanhoVetor);
        cout<<"Fator de carga: "<<setprecision(2)<<1.0-i<<endl;
        cout<<"Tamanho real do vetor: "<<tamanhoVetor<<endl;
        cout<<"Funcao Hash: Tamanho do vetor"<<endl;

        double start = getCurrentTimeInMillis();

        for(int j=0; j<n; j++)
        {
            //cout<<j<<endl;
            //hashInsereQuadratica(vetorNumeroAleatorio[j], V_AUX, tamanhoVetor);
            hashInsereLinear(vetorNumeroAleatorio[j], V_AUX, tamanhoVetor);
        }

        double end = getCurrentTimeInMillis();
        printTimeDiff(start, end);


       /* cout<<"**************************"<<endl;
        cout<<"Funcao Hash: proximo primo"<<endl;
        cout<<"Proximo primo: "<<PROXIMO_PRIMO<<endl;
        memset(V_AUX, 0, sizeof(V_AUX)); // Seta valores para 0
        start = getCurrentTimeInMillis();

        for(int j=0; j<n; j++)
        {
          hashInsereLinear(vetorNumeroAleatorio[j], V_AUX, tamanhoVetor, PROXIMO_PRIMO);
        }

        end = getCurrentTimeInMillis();
        printTimeDiff(start, end);

        cout<<"**************************"<<endl;
        cout<<"Funcao Hash: primo anterior"<<endl;
        cout<<"Primo anterior: "<<PRIMO_ANTERIOR<<endl;
        memset(V_AUX, 0, sizeof(V_AUX)); // Seta valores para 0
        start = getCurrentTimeInMillis();
        for(int j=0; j<n; j++)
        {
          hashInsereLinear(vetorNumeroAleatorio[j], V_AUX, tamanhoVetor, PRIMO_ANTERIOR);
        }

        end = getCurrentTimeInMillis();
        printTimeDiff(start, end);

        //imprimeVetor(V_AUX,tamanhoVetor);
        */
        cout << "*****************************"<< endl;
    }

    return 0;
}

void hashInsereLinear(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo)
{
    int chave = calculaHash(abs(elemento), tamanhoVetorHash);
    if(numeroPrimo != 0){
       int chave = calculaHash(elemento, numeroPrimo);
    }

    int posicao = chave;
    for(int i = 1; i <=tamanhoVetorHash; i++)
    {
        if (T[posicao] == INVALID_ARRAY_VALUE){
            T[posicao] = elemento;
            //cout<<"Insercao:: posicao="<<posicao<<", elemento:"<<elemento<<endl;
            return;
        }
        else if (T[posicao] == elemento){
            cout<<"Elemento repetido: "<<elemento<<endl;
            cout<<"Nao sera inserido!"<<endl;
            return;
        }

        // Percorre circular
        posicao = (chave + i) % tamanhoVetorHash;
    }

    cout<<"Percorrido n posicoes insercao invalida!"<<endl;
}

void hashInsereQuadratica(int elemento, int *T, int tamanhoVetorHash, int numeroPrimo)
{
    int chave = calculaHash(abs(elemento), tamanhoVetorHash);
    if(numeroPrimo != 0){
       int chave = calculaHash(elemento, numeroPrimo);
    }

    int posicao = chave;
    unsigned int somaQuadratica = 0;
    unsigned int incrementa=1;
    while(true)
    {
        if (T[posicao] == INVALID_ARRAY_VALUE){
            T[posicao] = elemento;
            //cout<<"Insercao:: posicao="<<posicao<<", elemento:"<<elemento<<endl;
            return;
        }
        else if (T[posicao] == elemento){
            cout<<"Elemento repetido: "<<elemento<<endl;
            cout<<"Nao sera inserido!"<<endl;
            return;
        }
        else
        {
           somaQuadratica =  incrementa * incrementa;
           //cout<<"Colisao: "<<somaQuadratica<<", i: "<<incrementa<<endl;
           incrementa++;
        }

        // Percorre circular
        posicao = (chave + somaQuadratica) % tamanhoVetorHash;
    }
}

int geraNumeroRandomico()
{
    this_thread::sleep_for(chrono::milliseconds(1));
    default_random_engine generator (chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(1, INT32_MAX);
    return distribution(generator);
}

int geraNumeroRandomicoRand(){
       return (rand() % INT32_MAX);
}

int calculaHash(int chave, int tamanho)
{
    return (chave % tamanho);
}

int numeroPrimoAnterior(int n){
    if (n<2){
        return 1;
    }

    while(!numeroEprimo(--n))
    { }
     return n;
}

int proximoPrimo(int n)
{
    while(!numeroEprimo(++n))
    { }
    return n;
}

bool numeroEprimo(int n){
    for (int i = 2; i < n; i++)
    {
        if (n % i == 0 && i != n) return false;
    }
    return true;
}

void imprimeVetor(int V[], int tamanho)
{
    cout<<"Vetor: "<<endl;
    for(int i=0; i<tamanho-1; i++)
    {
        cout<<V[i]<<", ";
    }
    cout<<V[tamanho-1]<<endl;
}

double getCurrentTimeInMillis() {
    return chrono::duration_cast<chrono::milliseconds>(
            chrono::steady_clock::now().time_since_epoch()
    ).count();
}

void printTimeDiff(double start, double end)
{
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
