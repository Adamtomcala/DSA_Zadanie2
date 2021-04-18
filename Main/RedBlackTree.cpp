/* DSA Zad2 .
 *  Prevzatá implementácia od spolužiaka
 *  Matej Vo¾anský, ID: 103180
 * */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BLACK_FLAG 0
#define RED_FLAG 1

typedef struct t_node {
    int value;
    char flag;
    struct t_node* left, * right, * parent;
}NODE;

NODE* create() {
    NODE* new_node = (NODE*)malloc(sizeof(NODE));
    return new_node;
}

/*int searchRB(NODE* search_node, int search_value) {
    return search_node == NULL ? 0
        : search_value < search_node->value ? searchRB(search_node->left, search_value)
        : search_value > search_node->value ? searchRB(search_node->right, search_value) : search_node->value;
}
*/
int searchRB(NODE* node, int number) {		//funkcia, ktora sluzi na vyhladavanie cisel v strome
    if (number < node->value) {						//ak je cislo mensie ako aktualny uzol										//vnorim sa vlavo
        searchRB(node->left, number);
    }
    else if (number > node->value) {					//ak je cislo vacsie ako aktualny uzol										//vnorim sa vpravo
        searchRB(node->right, number);
    }
    else if (number == node->value) {					//ak najdem dane cislo ,vypisem ho a aj jeho hlbku
        return node->value;
    }
    else {												//vypis ak sa cislo v strome nenachadza
        return 0;
    }
}
void recolor(NODE* start, NODE* grand_parent) {
    if (grand_parent == start) { grand_parent->flag = BLACK_FLAG; }
    else grand_parent->flag ^= 1;
    grand_parent->left->flag ^= 1;
    grand_parent->right->flag ^= 1;
}

void color_fix(NODE* sub_root) {
    sub_root->flag = BLACK_FLAG;
    if (sub_root->left) sub_root->left->flag = RED_FLAG;
    if (sub_root->right) sub_root->right->flag = RED_FLAG;
}

// Switch A parent with B child
NODE* left_rotation(NODE* root, NODE* sub_root) {
    NODE* temp_root = sub_root->right;
    temp_root->parent = sub_root->parent;
    NODE* sub_child = temp_root->left;
    temp_root->left = sub_root;
    sub_root->value = sub_root->value;
    sub_root->flag = sub_root->flag;
    sub_root->left = sub_root->left;
    sub_root->right = sub_child;
    sub_root->parent = temp_root;
    if (sub_child) sub_child->parent = sub_root;
    if (temp_root->parent && temp_root->value < temp_root->parent->value) temp_root->parent->left = temp_root;
    else if (temp_root->parent) temp_root->parent->right = temp_root;
    return sub_root->value == root->value ? temp_root : root;
}
// Switch B parent with A child
NODE* right_rotation(NODE* root, NODE* sub_root) {
    NODE* temp_root = sub_root->left;
    temp_root->parent = sub_root->parent;
    NODE* sub_child = temp_root->right;
    temp_root->right = sub_root;
    sub_root->value = sub_root->value;
    sub_root->flag = sub_root->flag;
    sub_root->left = sub_child;
    sub_root->right = sub_root->right;
    sub_root->parent = temp_root;
    if (sub_child) sub_child->parent = sub_root;
    if (temp_root->parent && temp_root->value < temp_root->parent->value) temp_root->parent->left = temp_root;
    else if (temp_root->parent) temp_root->parent->right = temp_root;
    return sub_root->value == root->value ? temp_root : root;
}

NODE* rotate(NODE* root, NODE* current_node, NODE* parent_node, NODE* grand_node) {
    // LL
    if (grand_node->left == parent_node && parent_node->left == current_node) {
        root = right_rotation(root, grand_node);
        color_fix(parent_node);
    }
    // RR
    else if (grand_node->right == parent_node && parent_node->right == current_node) {
        root = left_rotation(root, grand_node);
        color_fix(parent_node);
    }
    // LR
    else if (grand_node->left == parent_node && parent_node->right == current_node) {
        root = left_rotation(root, parent_node);
        root = right_rotation(root, grand_node);
        color_fix(current_node);
    }
    // RL
    else if (grand_node->right == parent_node && parent_node->left == current_node) {
        root = right_rotation(root, parent_node);
        root = left_rotation(root, grand_node);
        color_fix(current_node);
    }
    return root;
}

NODE* balance(NODE* root, NODE* check_node) {
    if (check_node->flag == RED_FLAG && check_node->parent->flag == RED_FLAG) {
        NODE* grand = check_node->parent->parent, * uncle = grand && grand->left == check_node->parent ? grand->right : grand->left;
        if (uncle && uncle->flag == RED_FLAG) { recolor(root, grand); return balance(root, grand); }
        else return rotate(root, check_node, check_node->parent, grand);
    }
    return root;
}

NODE* add_leaf(NODE* root, NODE* add, int insert_value) {
    if (!root && (root = add)) { return root; }
    NODE* act = root;
    while (!(insert_value < act->value && act->left == NULL || insert_value > act->value && act->right == NULL)) {
        if (act->value == insert_value) return root;
        act = insert_value < act->value ? act->left : act->right;
    }
    add->value = insert_value;
    add->flag = RED_FLAG;
    add->left = NULL;
    add->right = NULL;
    add->parent = act;
    if (insert_value < act->value) act->left = add; else act->right = add;
    return balance(root, add);
}

NODE* insertRB(NODE* root, int insert_value) {
    NODE* add = (NODE*)malloc(sizeof(NODE));
    add->value = insert_value;
    add->flag = BLACK_FLAG;
    add->left = NULL;
    add->right = NULL;
    add->parent = NULL;
    return add_leaf(root, add, insert_value);
}

void print_tree(NODE* root) {
    if (root != NULL) {
        if (root->parent != NULL)
            printf(root->parent->left != NULL && root->parent->left->value == root->value ? "Parent: %d Left Child: " : "Parent: %d RightChild: ", root->parent->value);
        else printf("Root: ");
        printf("%d Color: %s\n", root->value, root->flag ? "Red" : "Black");
        print_tree(root->left);
        print_tree(root->right);
    }
}

/* Tester will be like a for cycle with an array or random number like:
 *
 * */

NODE* test(NODE* root, int min_range, int max_range) {
    clock_t start = clock();
    for (int i = 0; i < (max_range - min_range); i++)
        root = insertRB(root, i);//( rand()% (max_range - min_range + 1)) + min_range);
    printf("\nInsertion of %d random numbers took %f ms\n", (max_range - min_range), (((double)clock() - start) / CLOCKS_PER_SEC) * 1000);
    double sum = 0;
    start = clock();
    for (int i = 0; i < (max_range - min_range); i++) {
        clock_t average = clock();
        searchRB(root, (rand() % (max_range - min_range + 1)) + min_range);
        sum += (((double)clock() - average) / CLOCKS_PER_SEC);
    }
    printf("Search for %d random numbers from range took %f ms\n", (max_range - min_range), (((double)clock() - start) / CLOCKS_PER_SEC) * 1000);
    printf("One search on average took %f ms\n", (double)(sum / (max_range - min_range)) * 1000);
    // DEBUG: print_tree(root);
    return root;
}
