#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "RedBlackTree.cpp"
#include "AVLtree.cpp"
#include "Hashovanie.cpp"
#include "HashovanieLinearProbing.cpp"

#pragma warning(disable : 4996)
int* arrrand(int size) {							//funckia, ktora sluzi na vygenerovanie
    int* arr = (int*)malloc(size * sizeof(int));		//cisel od 1 - size
    for (int i = 0; i < size; i++) {
        *(arr + i) = i + 1;
    }

    srand(time(NULL));
    for (int i = 0; i < size; i++) {					//nasledne sa cisla v poli random
        int randomIndex = rand() % size;				//rozhadzu
        int number = arr[i];
        arr[i] = arr[randomIndex];
        arr[randomIndex] = number;
    }

    return arr;											//vratenie pola
}

void testovac(int amountOfNumbers, int *arr) {
    NODE* root = NULL;
    AVLNODE* root1 = NULL;
    clock_t startAvl, endAvl, startRB, endRB;
    double time_takenAVL;
    double time_takenRB;
    int number;
    
    startRB = clock();
    for (int i = 0; i < amountOfNumbers; i++) {
        root = insertRB(root, arr[i]);
    }
    endRB = clock();
    time_takenRB = ((double)endRB - startRB) / CLOCKS_PER_SEC;

    printf("\nCas vkladania %d prvkov pri RB: %f sek.\n", amountOfNumbers, time_takenRB);

    startRB = clock();
    for (int i = 0; i < amountOfNumbers; i++) {
        number = searchRB(root, arr[i]);
    }
    endRB = clock();
    time_takenRB = ((double)endRB - startRB) / CLOCKS_PER_SEC;

    printf("Cas hladania vsetky prvkov pri pri: RB: %f sek.\n\n\n", time_takenRB);
    
    startAvl = clock();
    for (int i = 0; i < amountOfNumbers; i++) {
        root1 = insert(root1, arr[i]);
    }
    endAvl = clock();
    time_takenAVL = ((double)endAvl - startAvl) / CLOCKS_PER_SEC;

    printf("Cas vkladania %d prvkov pri AVL: %f sek.\n", amountOfNumbers, time_takenAVL);

    startAvl = clock();
    for (int i = 0; i < amountOfNumbers; i++) {
        number = search(root1, arr[i]);
    }
    endAvl = clock();
    time_takenAVL = ((double)endAvl - startAvl) / CLOCKS_PER_SEC;

    printf("Cas hladania vsetky prvkov pri AVL: %f sek.\n\n\n", time_takenAVL);
}

void testovacHash(int countOfNumbers, int *arr) {
    struct table* table = NULL;
    clock_t startHashCH, endHashCH;
    double resultTimeCH;
    int sizeOfTableCH = 11;
    int countInTable = 0;
    int number;

    table = init(table, countOfNumbers);

    startHashCH = clock();
    for (int i = 0; i < countOfNumbers; i++) {
        table = insertChaining(table, arr[i], &sizeOfTableCH, &countInTable);
    }
    endHashCH = clock();
    resultTimeCH = ((double)endHashCH - startHashCH) / CLOCKS_PER_SEC;
    printf("Cas vkladania %d prvkov pri Chaining-u: %f\n", countOfNumbers, resultTimeCH);

    startHashCH = clock();
    for (int i = 0; i < countOfNumbers; i++) {
        number = searchNumber(table, countInTable, sizeOfTableCH, arr[i]);
    }

    endHashCH = clock();
    resultTimeCH = ((double)endHashCH - startHashCH) / CLOCKS_PER_SEC;
    printf("Cas hladania vsetky prvkov pri Chainung-u: %f\n\n\n", resultTimeCH);
}


void testovacLinear(int sizeOfLinear, int *arr, int countOfNumbers) {
    arrHash = (int *) calloc(sizeOfLinear, sizeof(int));
    countLinear = 0;
    clock_t start, end;
    double resultTime;
    start = clock();
    for (int i = 0; i < countOfNumbers; i++) {
        Insert(arr[i]);
    }
    end = clock();
    resultTime = ((double)end - start) / CLOCKS_PER_SEC;

    printf("Cas vkladania %d prvkov pri Linear Probing: %f\n", countOfNumbers, resultTime);

    start = clock();
    for (int i = 0; i < countOfNumbers; i++) {
        SearchLinearProbing(arr[i]);
    }
    end = clock();
    resultTime = ((double)end - start) / CLOCKS_PER_SEC;

    printf("Cas hladania vsetky prvkov pri Linear Probing: %f\n\n\n", resultTime);

    free(arrHash);
}

int main() {
    int* arr;
    int flag = 1;
    int cisloTestu;
    arr = arrrand(10000000);

    printf("Testovacie scenare su zamerane na zistenie rychlosti funkcii insert a search:\n");
    printf("1. scenar: Vytvorenie struktur o velkosti 100 000 a priradenie nahodnych hodnot od 1 - 5 000 000\n");
    printf("2. scenar: Vytvorenie struktur o velkosti 1 000 000  a priradenie nahodnych hodnot od 1 - 5 000 000\n");
    printf("3. scenar: Vytvorenie struktur o velkosti 5 000 000 prvkov nahodnych od 1 - 5 000 000 \n");
    printf("Pre ukoncenie stlacte 0.\n");

    while (flag) {
        printf("Zadajte cislo testu: ");
        scanf("%d", &cisloTestu);
        switch (cisloTestu){
            case 1: 
                testovac(100000, arr);
                testovacHash(100000, arr);
                sizeOfLinear = 200000;
                testovacLinear(sizeOfLinear, arr, 100000);
                break;
            case 2:
                testovac(1000000, arr);
                testovacHash(1000000, arr);
                sizeOfLinear = 1500000;
                testovacLinear(sizeOfLinear, arr, 1000000);
                break;
            case 3: 
                testovac(5000000, arr);
                testovacHash(5000000, arr);
                sizeOfLinear = 10000000;
                testovacLinear(sizeOfLinear, arr, 5000000);
                break;
            case 0:
                flag = 0;
                break;
            default:
                printf("Nespravne cislo testu.\n\n");
                break;
        }
    }
	return 0;
}