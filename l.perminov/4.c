#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256

typedef struct Node {
    char* data;
    struct Node *next;
} Node;

void insert(Node** head, const char* str) {

    Node* new_node = (Node*)malloc(sizeof(Node));

    new_node->data = (char*)malloc(strlen(str) + 1);

    strcpy(new_node->data, str);

    new_node->next = *head;

    *head = new_node;
}

void print_list(Node* head) {
    Node* current = head;

    while (current != NULL) {
        printf("%s", current->data);

        current = current->next;
    }
}

void free_list(Node* head) {
    Node* current = head;

    while (current != NULL) {
        Node* next = current->next;

        free(current->data);
        free(current);

        current = next;
    }
}

int main() {
    Node* head = NULL;

    char buffer[MAX_LENGTH];

    while (1) {
        if (buffer[0] == '.') {
            break;
        }

        insert(&head, buffer);
    }

    printf("\nEntered strings:\n");
    print_list(head);

    free_list(head);

    return 0;
}