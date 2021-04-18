#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    int number;
    int height;
    struct node* right, * left;
} AVLNODE;

AVLNODE* create(int number) {									//funkcia, ktora vytvori novy uzol
    AVLNODE* new_node = (AVLNODE*)malloc(sizeof(AVLNODE));			//mallocne sa novy struktura
    new_node->number = number;								//priradi sa mu dana hodnota
    new_node->left = new_node->right = NULL;				//jeho pravy a lavy podstrom nastavim NULL
    new_node->height = 1;

    return new_node;										//vratim novovytvoreny uzol
}

int heightOfNode(AVLNODE* root) {								//funkcia, ktora sluzi na najdenie vysky daneho uzla
    int left, right, height;

    left = (root->left == NULL) ? 0 : root->left->height;		//skontorluje pravu stranu
    right = (root->right == NULL) ? 0 : root->right->height;	//skontorluje lavu stranu
    height = (left > right) ? left : right;						//vyberie vacsie z nich
    height++;													//zvacsim o 1
    return height;												//vraati vysku
}

AVLNODE* left(AVLNODE* root) {								//funckia, pomocou ktorej vykonavam rotaciu vlavo
    AVLNODE* x = root;										//x bude tvorit pravu casz y
    AVLNODE* y = root->right;								//y bude uzol, ktory vraciam
    AVLNODE* z = root->right->right;						//z bude tvorit pravu cast y;

    x->right = y->left;									//vymena ukazovatelov
    y->left = x;

    x->height = heightOfNode(x);						//zistenie novych vysok

    y->height = heightOfNode(y);

    return y;											//vraciam vybalansovany uzol
}


AVLNODE* left2(AVLNODE* root) {							//fukcia, pomocou ktorej vykonovam rotaciu vlavo a prvok,
    //ktory vkladam je mensi ako aktualny uzol
    AVLNODE* x = root;									//inicializujem si jednotlive uzly
    AVLNODE* y = root->right->left;
    AVLNODE* z = root->right;
    AVLNODE* temp;

    if(y->left != NULL || y->right != NULL){
        x->right = y->left;
        z->left = y->right;
        y->left = x;
        y->right = z;
    }
    else {
        temp = NULL;
        z->left = temp;									//vymeny ukazavatelov, aby bol strom vybalansovany
        x->right = NULL;
        y->left = x;
        y->right = z;
    }


    z->height = heightOfNode(z);					//vypocitanie novych vysok pre dane uzly

    x->height = heightOfNode(x);

    y->height = heightOfNode(y);

    return y;										//vraciam vybalansovany uzol
}
AVLNODE* right(AVLNODE* root) {							//funckia, pomocou ktorej vykonavam rotaciu vpravo
    AVLNODE* x = root->left->left;						//inicializacia jednotlivych uzlov
    AVLNODE* y = root->left;
    AVLNODE* z = root;

    z->left = y->right;								//vymeny ukazovatelov, aby bol strom vybalansovany
    y->right = z;

    z->height = heightOfNode(z);					//vypocet novych vysok

    y->height = heightOfNode(y);

    return y;										//vratenie vybalansovaneho uzla
}

AVLNODE* right2(AVLNODE* root) {							//rotacia vpravo a vkladany prvok je vacsi ako uzol
    AVLNODE* x = root->left;							//inicializacia jednotlivych uzlov
    AVLNODE* y = root->left->right;
    AVLNODE* z = root;
    AVLNODE* temp;

    if (y->left != NULL || y->right != NULL) {
        z->left = y->right;
        x->right = y->left;
        y->left = x;
        y->right = z;
    }
    else {
        temp = NULL;
        x->right = temp;								//vymeny jednotlivych uzlov
        z->left = NULL;
        y->left = x;
        y->right = z;
    }



    z->height = heightOfNode(z);					//vypocet jednotlivych vysok

    x->height = heightOfNode(x);

    y->height = heightOfNode(y);

    return y;										//vratenie vybalansovaneho uzla
}
int* randomArray(int size) {							//funckia, ktora sluzi na vygenerovanie
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


int search(AVLNODE* node, int number) {		//funkcia, ktora sluzi na vyhladavanie cisel v strome
    if (number < node->number) {						//ak je cislo mensie ako aktualny uzol									//vnorim sa vlavo
        search(node->left, number);
    }
    else if (number > node->number) {					//ak je cislo vacsie ako aktualny uzol									//vnorim sa vpravo
        search(node->right, number);
    }
    else if (number == node->number) {					//ak najdem dane cislo ,vypisem ho a aj jeho hlbku
        return node->number;
    }
    else {												//vypis ak sa cislo v strome nenachadza
        return 0;
    }
}
AVLNODE* insert(AVLNODE* node, int number) {						//funckia, ktora sluzi na vkladanie do stromu
    AVLNODE* temp1, * temp2;

    if (node == NULL) {										//ak je uzol NULL, vytvori sa novy uzol
        node = create(number);
        return node;
    }

    if (number < node->number) {							//ak je cislo mensie ako aktualny uzol, vnorim sa dolava
        node->left = insert(node->left, number);
    }
    else if (number > node->number) {						//ak je cislo vacsie ako aktualny uzol, vnorim sa doprava
        node->right = insert(node->right, number);
    }

    //pri vynarani z rekurzie je dolezite aktualizovat vysky jednotlivych uzlov a zistit ci je uzol vybalansovany

    int leftHeight, rightHeight;

    leftHeight = (node->left == NULL) ? 0 : node->left->height;		//zistim si vysku lavej casti
    rightHeight = (node->right == NULL) ? 0 : node->right->height;	//zistim si vysku pravej casti

    node->height = (leftHeight > rightHeight) ? leftHeight : rightHeight;	//porovnam ich a veberiem vasciu z nich
    node->height++;													//zvacsim ju o 1

    int balanceFactor;												//vypocitam si balanceFactor ako rozdiel lavej a
    balanceFactor = leftHeight - rightHeight;						//pravej vysky

    //musim riesit nasledujuce situacie ak balanceFactor nie je v mnozine {-1,0,1}

    if (balanceFactor < -1) {										//ak je balanceFactor < -1 a vkladani cislo je
        if (number > node->right->number) {							//vacsie ako cislo v pravej casti podstromu
            return left(node);										//vykonam rotaciu vlavo
        }
        else {														//ak je cislo mensie musim vykonaz este jedno otocenie
            return left2(node);										//po vymene uzlo vykonam este rotaciu uzla vlavo
        }
    }
    else if (balanceFactor > 1) {									//ak je balanceFactor > 1 a vkladani cislo je
        if (number < node->left->number) {							//mensie ako cislo v lavej casti podstromu
            return right(node);										//vykonam rotaciu vpravo
        }
        else {														//ak je heslo vyssie musim vykonat eete jedno otocenie
            return right2(node);									//po vymene uzlov vykonam este rotaciu uzla vpravo
        }
    }
    else {
        return node;
    }
}

void printInorder(AVLNODE* node) {								//funckia, ktore vypise strom inorder
    if (node == NULL)
        return;

    printInorder(node->left);

    printf("%d (height %d)", node->number, node->height);

    printInorder(node->right);
}

