#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef struct table {			//struktura tabulky
	int data;
	struct table* next;
};

struct table* insertChaining(struct table* table, int key, int *sizeOfTable, int *countOfNumbers);
struct table* init(struct table* table, int sizeOfTable);
int hashFuction(int key, int sizeOfTable);
int findPrime(int sizeOfTable);
struct table* rehash(struct table* table, int *sizeOfTable, int *countOfNumbers);

struct table* init(struct table* table, int sizeOfTable) {				//funkcia, ktora sluzi na inicializaciu tabulky
	table = (struct table*)malloc(sizeOfTable * sizeof(struct table));	//vytvori sa tabulka danej velkosti
	for (int i = 0; i < sizeOfTable; i++) {								//kazdemu indexu sa priradi cislo 1, ktore
		table[i].data = -1;												//znaci to, ze tam nie je ulozeny ziadny prvok
		table[i].next = NULL;
	}
	return table;
}

int hashFuction(int key, int sizeOfTable) {					//funkcia, ktora vracia hash kod
	return (key % sizeOfTable);								//modulo cislo a velkosti tabulky
}

int findPrime(int sizeOfTable) {							//funkcia, ktora vrati nove prvocislo
	int prime = sizeOfTable*2;								//ktore bude pouzite ako nova velkost
	int flag;												//hashovacej tabulky		
	while (1) {												//nove prvocislo bude aspon 2krat
		for (int i = 2; i < sqrt(prime); i++) {				//vacsie ako povodna velkost tabulky
			if (prime % i == 0) {
				flag = 0;
				break;
			}
			else {
				flag = 1;
			}
		}
		if (flag) {
			break;
		}
		prime++;
	}
	return prime;
}

struct table* rehash(struct table* table, int *sizeOfTable, int *countOfNumbers) {	//funkcia, ktora sluzi na rehashovanie
	int newSize = findPrime(*sizeOfTable);											//tabulky po tom ako sa zvacsila			
	struct table* newTable = NULL;
	newTable = init(newTable, newSize);				//inicializacia novej tabulky novej velkosti
	struct table* temp;
	int newCountOfTable = 0;

	for (int i = 0; i < *sizeOfTable; i++) {		//cyklus, ktory sluzi na prehladavanie starej tabulky
		if (table[i].data == -1) {					//ak tam je -1 nie je tam ulozena ziadna hodnota
			continue;					
		}
		newTable = insertChaining(newTable, table[i].data, &newSize, &newCountOfTable);	//ak nie je tam hodnota a vlozim ju do 
																						//novej tabulky
		temp = table[i].next;						//v starej tabulke kontrolujem aj prvky, ktore mozu by za sebou v retazi
		while (temp != NULL) {						//ak tam nejaky je tak ho vlozim do novej tabulky
			newTable = insertChaining(newTable, temp->data, &newSize, &newCountOfTable);
			temp = temp->next;
		}
	}
	*sizeOfTable = newSize;							//aktualizujem velkost tabulky
	*countOfNumbers = newCountOfTable;				//pocet prvkov

	return newTable;								//vratim novu tabulku
}

struct table* insertChaining(struct table* table, int key, int *sizeOfTable, int *countOfNumbers) {
	int size = *sizeOfTable;					//funkcia, ktora sluzi na pridanie cisla do tabulky
	int count = *countOfNumbers;
	int index = hashFuction(key, size);			//najdenie hashkodu

	if (table[index].data == -1) {				//ak je na danom indexxe -1 znamena to, ze tam je volno
		table[index].data = key;				//pridam tam dane cislo
		count++;
	}
	else {										//ak tam nie je volne miesto, vnorim sa dalej v danom indexe
		struct table* temp = &table[index];
		while (temp->next != NULL) {
			temp = temp->next;
		}										//vnaram sa az kym nenajdem NULL pointer
		struct table* newNumber = (struct table*)malloc(sizeof(struct table));		//vytvorim nove miesto
		newNumber->data = key;					//a vlozim dane cislo
		newNumber->next = NULL;
		temp->next = newNumber;
		count++;
	}
	double alfa = (double)count / size;
	if (alfa > 0.5) {							//skontrolujem ci nie je zaplnene viac ako polovica tabulky
		table = rehash(table, &size, &count);	//ak je tak rehashujem tabulku 
	}
	*sizeOfTable = size;
	*countOfNumbers = count;
	return table;
}

//funkcia na hladanie cisla v tabulke
int searchNumber(struct table* table, int countOfNumbers, int sizeOfTable, int key) {
	int index = hashFuction(key, sizeOfTable);		//zistime hash daneho cisla
	if (table[index].data == -1) {					//ak je na danom -1 cislo sa v 
		printf("Cislo %d sa nenachadza v tabulke.\n", key);		//tabulke nenachadza
		return -1;
	}
	if (table[index].data != key) {					//ak sa tam nachadza ine cislo
		struct table* temp = &table[index];			//idem kontrolovat retazenie
		while (temp->data != key) {
			temp = temp->next;
			if (temp == NULL) {						//ak sa temp == NULL, dane cislo tam nie je
				printf("Cislo %d sa nenachadza v tabulke.\n", key);
				return -1;
			}
		}
		return temp->data;
	}
	return table[index].data;
}
