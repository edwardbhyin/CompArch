#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    int data;
    struct node *next;
};

struct node *head = NULL;
int listLength = 0;

void printList() {
    struct node *p = head;
    while (p != NULL) {
        printf(" %i", p->data);
        p = p->next;
    }
    printf("\n");
}

void insert (int data) {
    struct node *new = (struct node*)malloc(sizeof(struct node));
    new->data = data;

    if (head == NULL) {
        head = new;
        head->next = NULL;
        listLength++;
        return;
    }
    if (listLength == 1) {
        if (data == head->data) {
            free(new);
            return;
        }
        if (data > head->data) {
            head->next = new;
            new->next = NULL;
            listLength++;
            return;
        } else if (data < head->data) {
            new->next = head;
            head = new;
            listLength++;
            return;
        }
    }
    if (data < head->data) {
        new->next = head;
        head = new;
        listLength++;
        return;
    }
    struct node *temp = head->next, *prev = head;
    while (temp != NULL) {
        if (data == temp->data || data == prev->data) {
            free(new);
            return;
        }
        if (data < temp->data && data > prev->data) {
            prev->next = new;
            new->next = temp;
            listLength++;
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        prev->next = new;
        new->next = NULL;
        listLength++;
    }
}

void delete(int key){
   struct node *temp = head, *prev;
   if (temp != NULL && temp->data == key) {
      head = temp->next;
      free(temp);
      listLength--;
      return;
   }

   while (temp != NULL && temp->data != key) {
      prev = temp;
      temp = temp->next;
   }

   if (temp == NULL) return;

   prev->next = temp->next;
   free(temp);
   listLength--;
}

int main(int argc, char *argv[]) {
    int loop = 0;
    while(loop == 0){
        char *input = (char *)malloc(100*sizeof(char));
        int n = scanf("%[^\n]%*c", input);
        if (n == EOF || n == 0) {
        loop = 1;
        free(input);
        break;
        }
        char *text = (char *)malloc(100*sizeof(char));
        int len = (int) strlen(input);
        for(int i = 2; i < len ; i++) {
            text[i-2] = input[i];
        }
        if(input[0] == 'i') {
            int data;
            sscanf(text, "%d", &data);
            insert(data);
            printf("%i :",listLength);
            printList();
        } else if (input[0] == 'd') {
            int key;
            sscanf(text, "%d", &key);
            delete(key);
            printf("%i :",listLength);
            printList();
        }
        free(input);
        free(text);
    }
    return 0;
}