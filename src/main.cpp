//
// Created by wgalvao on 11/26/17.
//

/* HASHING - LINEAR AND QUADRATIC PROBING */

#include <stdio.h>
#include <iostream>

using namespace std;
int tsize = 11;

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
    int n = 4;
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

int main()
{
    //squad_insert();
    linear_insert();

    return 0;
}