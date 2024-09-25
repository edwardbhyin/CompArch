#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct node {
    int data;
    struct node *right_child;
    struct node *left_child;
};

struct node* search(struct node *root, int key) {
    if (root == NULL || root->data == key)
        return root;
    else if (key > root->data)
        return search(root->right_child, key);
    else
        return search(root->left_child, key);
}

struct node* new_node(int data) {
    struct node *p;
    p = malloc(sizeof(struct node));
    p->data = data;
    p->left_child = NULL;
    p->right_child = NULL;

    return p;
}

struct node* insert(struct node *root, int data) {
    if (root == NULL)
        return new_node(data);
    else if (data > root->data)
        root->right_child = insert(root->right_child, data);
    else
        root->left_child = insert(root->left_child, data);
    return root;
}

void printTree(struct node *root) {
    if (root != NULL) {
        printf("(");
        printTree(root->left_child);
        printf("%i", root->data);
        printTree(root->right_child);
        printf(")");
    }
}

struct node* find_minimum(struct node *root) {
    if (root == NULL)
        return NULL;
    else if (root->left_child != NULL)
        return find_minimum(root->left_child);
    return root;
}

struct node* delete(struct node *root, int key) {
    if (root == NULL)
        return NULL;
    if (key > root->data)
        root->right_child = delete(root->right_child, key);
    else if (key < root->data)
        root->left_child = delete(root->left_child, key);
    else {
        if (root->left_child == NULL && root->right_child == NULL) {
            free(root);
            return NULL;
        } else if (root->left_child == NULL || root->right_child == NULL) {
            struct node *temp;
            if (root->left_child == NULL)
                temp = root->right_child;
            else
                temp = root->left_child;
            free(root);
            return temp;
        } else {
            struct node *temp = find_minimum(root->right_child);
            root->data = temp->data;
            root->right_child = delete(root->right_child, temp->data);
        }
    }
    return root;
}

struct node *root = NULL;

int main() {
    while (1) {
        char input[100];
        if (scanf("%99[^\n]%*c", input) == EOF) {
            break;
        }

        char command = input[0];

        if (command == 'i') {
            int data;
            if (sscanf(input + 2, "%d", &data) != 1) {
                printf("Invalid input for 'i' command\n");
            } else {
                struct node *result = search(root, data);
                if (result && result->data == data) {
                    printf("not inserted\n");
                } else {
                    root = insert(root, data);
                    printf("inserted\n");
                }
            }
        } else if (command == 's') {
            int key;
            if (sscanf(input + 2, "%d", &key) != 1) {
                printf("Invalid input for 's' command\n");
            } else {
                struct node *result = search(root, key);
                if (result && result->data == key) {
                    printf("present\n");
                } else {
                    printf("absent\n");
                }
            }
        } else if (command == 'p') {
            printTree(root);
            printf("\n");
        } else if (command == 'd') {
            int key;
            if (sscanf(input + 2, "%d", &key) != 1) {
                printf("Invalid input for 'd' command\n");
            } else {
                struct node *result = search(root, key);
                if (result && result->data == key) {
                    printf("deleted\n");
                    root = delete(root, key);
                } else {
                    printf("absent\n");
                }
            }
        } else {
            printf("Invalid command\n");
        }
    }

    return 0;
}
