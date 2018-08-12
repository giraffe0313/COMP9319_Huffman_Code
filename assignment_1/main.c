//
//  main.c
//  assignment_1
//
//  Created by Wujian on 10/8/18.
//  Copyright © 2018 Wujian. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked_list.h"

int *select_sort(int *array, int length);

Tree_node* Tree_node_init(Tree_node* new, int val, int frequency) {
    new -> val = val;
    new -> frequency = frequency;
    new -> left = NULL;
    new -> right = NULL;
    new -> depth = 0;
    memset(new -> code, 0, 16);
    new -> position = 0;
    return new;
}

Tree_node *construct_huffman(list_node *head);

void add_bit(char* bittarray, int k);
void DFS(Tree_node *root);
void char_print(char* a);

int main(int argc, const char * argv[]) {
    
     const char file_pwd[] = "/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/test1.txt";
//    const char file_pwd[] = "/import/adams/3/z5103624/COMP9319/assignment1/test1.txt";
    FILE *file = fopen(file_pwd, "r");
    int get_result;
    int distinct_num = 0;
    int frequence[127] = {0};
    int i;
    // open file
    while ((get_result = fgetc(file)) != EOF) {
        if (! frequence[get_result]) {
            distinct_num++;
        }
        frequence[get_result]++;
    }
    fclose(file);
    
    // init linked list head
    list_node *head = malloc(sizeof(list_node));
    head -> next = NULL;
    head -> frequency = 0;
    
    // counting frequency and construct linked list
    for (i = 0; i < 127; i++) {
        if (frequence[i]) {
//            printf("array: char is %c, frequency is %d\n", i, frequence[i]);
            Tree_node *new = malloc(sizeof(Tree_node));
            new = Tree_node_init(new, i, frequence[i]);
            head = list_insert_node(head, i, frequence[i], new);
//            list_print(head);
//            printf("\n");
        }
    }
    list_print(head);
    
    // construct huffman tree
    Tree_node *root = construct_huffman(head);
    DFS(root);
    printf("\n");
    
    printf("%d\n", root ->position);
    
    return 0;
}

Tree_node *construct_huffman(list_node *head) {
    list_node *list_temp_head = head;
    Tree_node *tree_node_temp = malloc(sizeof(Tree_node));
    tree_node_temp = Tree_node_init(tree_node_temp, -1, 0);
    Tree_node *root = tree_node_temp;
    while (list_temp_head) {
        printf("list frequency name is %d, value is %d\n",list_temp_head -> name, list_temp_head ->frequency);
    
        if (!tree_node_temp -> left) {
            tree_node_temp -> left = list_temp_head -> tree_node;
            list_temp_head -> tree_node -> position = 0;
        } else if (!tree_node_temp -> right) {
            tree_node_temp -> right = list_temp_head -> tree_node;
            list_temp_head -> tree_node -> position = 1;
            tree_node_temp -> frequency = tree_node_temp -> left -> frequency + tree_node_temp -> right -> frequency;
            list_temp_head = list_insert_node(list_temp_head, -1, tree_node_temp -> frequency, tree_node_temp);
            root = tree_node_temp;
            tree_node_temp = malloc(sizeof(Tree_node));
            tree_node_temp = Tree_node_init(tree_node_temp, -1, 0);
        }
        list_temp_head = list_temp_head -> next;
    }
    return root;
}

void DFS(Tree_node *root) {
    if (!root) {
        return;
    }
//    if (root -> left || root -> right) {
//        printf("%d ", root -> position);
//    }
//    else {
//        printf("%d %d ", root -> position, root -> val);
//        return;
//    }
//
//    DFS(root -> left);
//    printf("-1 ");
//    DFS(root -> right);
//    printf("-1 ");
    
    if ((!root -> left) && (!root -> right)) {
        // printf("%c: %d\n", root -> val, root -> code[0]);
        printf("%d: depth is %d, code is ",root -> val, root -> depth);
        char_print(&(root -> code[0]));
        printf("\n");
        return;
    }
    
    if (root -> left) {
        // printf("left: 0 and %d  ", root -> position);
        root -> left -> depth = root -> depth + 1;
        
        int i;
        for (i = 0; i < 16; i++) {
            root -> left -> code[i] = root -> code[i];
        }
        
        DFS(root -> left);
    }
    
    // printf("-1 ");
    if (root -> right) {
        // printf("right: 1 and %d ", root -> position);
        root -> right -> depth = root -> depth + 1;
        int i;
        for (i = 0; i < 16; i++) {
            root -> right -> code[i] = root -> code[i];
        }
        add_bit(root -> right -> code, root -> depth);
//        char_print(&(root -> code[0]));

        DFS(root -> right);
    }
    // printf("-1 ");

    
}



int *select_sort(int *array, int length) {
    int index_of_min;
    int i, j;
    for (i = 0; i < length - 1; i++) {
        index_of_min = i;
        for (j = i+1; j < length; j++) {
            if (array[j] < array[index_of_min]) {
                index_of_min = j;
            }
        }
        if (index_of_min != i) {
            int temp = array[i];
            array[i] = array[index_of_min];
            array[index_of_min] = temp;
        }
    }
    return array;
}

void add_bit(char* bittarray, int k) {
    int i = k / 8;
    int j = k % 8;
    bittarray[i] = ((0x80 >> j) | bittarray[i]);
}

void char_print(char* a) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("%d", !!((*a << i) & 0x80));
    }
};
