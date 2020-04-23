#include<stdio.h>
#include<stdlib.h>

typedef struct dll_node {
    int value;
    struct dll_node * prev;
    struct dll_node * next;
} dll_node;

void new_node(dll_node *curr_p, double val) {
    (*curr_p).next = (dll_node *)malloc(sizeof(struct dll_node));
    struct dll_node * new_p = (*curr_p).next;
    (*new_p).value = val;
    (*new_p).prev = curr_p;
}

void print_list(dll_node * root) {
    struct dll_node * curr_p = root;
    while (curr_p != NULL) {
        printf("%d\n",(*curr_p).value);
        curr_p = (*curr_p).next;
    }
}
void print_list_backwards(dll_node * root_p) {
    struct dll_node * curr_p = root_p;
    struct dll_node * last_p = NULL;
    while (curr_p != NULL) {
        last_p = curr_p;
        curr_p = (*curr_p).next;
    }
    curr_p = last_p;
    while (curr_p != NULL) {
        printf("%d\n",(*curr_p).value);
        curr_p = (*curr_p).prev;
    }
}
void copy_list(dll_node * root1_p, dll_node * root2_p) {
    struct dll_node *curr2_p = root2_p;
    struct dll_node *curr1_p = (*root1_p).next;

    int n = 0;
    while (curr1_p != NULL) {
        if (n+1 == 1) {
            (*root2_p).value = (*root1_p).value;
        }
        else {
            new_node(curr2_p,(*curr1_p).value);
            curr2_p = (*curr2_p).next;
            curr1_p = (*curr1_p).next;
        }
        n++;
    }
}
void swap_nodes(dll_node * node1_p, dll_node * node2_p) {
    int hold;
    hold = (*node1_p).value;
    (*node1_p).value = (*node2_p).value;
    (*node2_p).value = hold;
   
}

int main() {
    int i,num=0; 
    int n = 0;
    struct dll_node root;
    struct dll_node root2;
    root2.prev = NULL;
    struct dll_node *curr_p;
    curr_p = &root;

    while (scanf(" %d",&num) == 1) {
        if (n+1 == 1) {
            root.value = num;
        }
        else {
            new_node(curr_p,num);
            curr_p = (*curr_p).next;
        }
        n++;
    }
    if (n == 1) printf("Read 1  integer\n");
    else        printf("Read %d integers\n",n);

    copy_list(&root,&root2);

    for (i = 0; i < n+1; i++) {
        curr_p = &root;
        while ((*curr_p).next != NULL) {
            if ((*curr_p).value > (*(*curr_p).next).value) {
                swap_nodes(curr_p, (*curr_p).next);
            }
            curr_p = (*curr_p).next;
        }
    }

    print_list(&root);

    for (i = 0; i < n+1; i++) {
        curr_p = &root;
        while ((*curr_p).next != NULL) {
            if ((*curr_p).value < (*(*curr_p).next).value) {
                swap_nodes(curr_p, (*curr_p).next);
            }
            curr_p = (*curr_p).next;
        }
    }

    print_list(&root);
    print_list_backwards(&root2);
}
