//
//  huffman.h
//  assignment_1
//
//  Created by Wujian on 13/8/18.
//  Copyright © 2018 Wujian. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifndef huffman_h
#define huffman_h

typedef struct _Tree_node {
    int frequency;
    int val;
    int position;
    int depth;
    char code[16];
    struct _Tree_node *left;
    struct _Tree_node *right;
    struct _Tree_node *parent;
} Tree_node;

typedef struct _list_node {
    int name;
    int frequency;
    Tree_node *tree_node;
    struct _list_node *next;
} list_node;

struct code_information {
    int exist_or_not;
    Tree_node* code;
};

typedef struct {
    int length;          // the number of bytes in header's bitarray
    int number_of_bits;  // the number of encode bits
    char bitarray[1016];
} Header;


// huffman tree functions
Tree_node* Tree_node_init(Tree_node* new, int val, int frequency);

list_node *construct_linked_list(int *frequence);

Tree_node *construct_huffman(list_node *head);
void DFS(Tree_node *root, struct code_information* code_list, int *total_bit, Header *header);
// void DFS_for_decode(Tree_node *root);

Tree_node *back_trace(Tree_node *node);

// bit operations
void add_bit(char* bittarray, int k);
void char_print(char* a, int k);
int check_target_binary(char *a, int k);  

// linked list functions
list_node *list_insert_node(list_node *head, int name, int frequency, Tree_node *tree_node);
void list_print(list_node *head);
void list_clean(list_node *head);

// decode function
int get_one_decode_result(FILE* file, int *number_of_bits, int *bit_index, int *loop_indicator, int *get_result, Tree_node *recover_root);

// kmp function
void prefix_table(char pattern[], int prefix[], int n);

#endif /* huffman_h */
