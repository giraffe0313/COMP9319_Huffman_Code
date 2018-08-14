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
#include "huffman.h"

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

struct code_information {
    int exist_or_not;
    Tree_node* code;
};

typedef struct {
    int length;
    char bitarray[1020];
} Header;

Tree_node *construct_huffman(list_node *head);

void add_bit(char* bittarray, int k);
void DFS(Tree_node *root, struct code_information* code_list, int *total_bit, Header *header);
void char_print(char* a, int k);
int check_target_binary(char *a, int k);


int main(int argc, const char * argv[]) {
    
    const char file_pwd[] = "/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/test2.txt";
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
            Tree_node *new = malloc(sizeof(Tree_node));
            new = Tree_node_init(new, i, frequence[i]);
            head = list_insert_node(head, i, frequence[i], new);
        }
    }
    list_print(head);
    
    // construct huffman tree
    Tree_node *root = construct_huffman(head);

    // with pointer points the corresponding tree node
    struct code_information* code_list = malloc(128 * sizeof(struct code_information));
    memset(code_list, 0, 128 * sizeof(struct code_information));

    int total_bit = 0;  // the number of encode bit

    // header with 1024 bytes
    Header header;
    header.length = 0;
    memset(header.bitarray, 0, 1020);

    DFS(root, code_list, &total_bit, &header);
    printf("\n");

    for (i = 0; i < 128; i++) {
        if (code_list[i].exist_or_not) {
            printf("val is %d, code is ", i);
            char_print(code_list[i].code -> code, 0);
            printf("\n");
        }
    }
    
    printf("%d\n", root ->position);
    printf("total bit is %d\n", total_bit);
    
    
//    start encode
    int number_of_bytes_needed = total_bit / 8;
    if (number_of_bytes_needed % 8) {
        number_of_bytes_needed++;
    }
    char *encode_result = malloc(number_of_bytes_needed * sizeof(char));
    
    file = fopen(file_pwd, "r");
    
    int total_number = 0;
//    int inner_number = 0;
    while ((get_result = fgetc(file)) != EOF) {
        if (!code_list[get_result].exist_or_not) {
            printf("bug!!!!!!]\n");
        }
        for (i = 0; i < code_list[get_result].code -> depth; i++) {
            printf("debug print: get_result is %d, depth is %d, code is ", get_result, code_list[get_result].code -> depth);
            char_print(code_list[get_result].code -> code, 0);
            printf("\n");
            if (check_target_binary(code_list[get_result].code -> code, i)) {
                add_bit(encode_result, total_number);
            }
            total_number++;
        }
    }
    fclose(file);
    
    for (int i = 0; i < total_bit; i++) {
        // char_print(encode_result, i/8);
        if (check_target_binary(encode_result, i)) {
            printf("1 ");
        } else {
            printf("0 ");
        }
    }

    file = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/output.huffman", "wb");
    fwrite(encode_result, sizeof(char), number_of_bytes_needed, file);
    fclose(file);

    file = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/header.test", "wb");
    fwrite(&header, sizeof(Header), 1, file);
    fclose(file);
    return 0;
}





void DFS(Tree_node *root, struct code_information* code_list, int *total_bit, Header *header) {
    // reach leaf node
    if ((!root -> left) && (!root -> right)) {
        // printf("%c: %d\n", root -> val, root -> code[0]);
//        printf("\n%d: depth is %d, frequency is %d, code is ",root -> val, root -> depth, root -> frequency);
//        char_print(&(root -> code[0]), 0);
//        printf("\n");
        code_list[root -> val].exist_or_not = 1;
        code_list[root -> val].code = root;
        *total_bit = *total_bit + (root -> frequency) * (root -> depth);
//        printf("total bit: %d\n", *total_bit);
        printf("1 %d ", root -> val);
        header -> bitarray[header -> length] = 1;
        header -> bitarray[header -> length + 1] = root -> val;
        header -> length = header -> length + 2;

        return;
    }
    
    if (root -> left) {

        printf("0 ");
        header -> bitarray[header -> length] = 0;
        header -> length++;

        root -> left -> depth = root -> depth + 1;
        
        int i;
        for (i = 0; i < 16; i++) {
            root -> left -> code[i] = root -> code[i];
        }
        
        DFS(root -> left, code_list, total_bit, header);
    }
    
    // printf("-1 ");
    if (root -> right) {
        printf("0 ");
        header -> bitarray[header -> length] = 0;
        header -> length++;

        root -> right -> depth = root -> depth + 1;
        int i;
        for (i = 0; i < 16; i++) {
            root -> right -> code[i] = root -> code[i];
        }
        add_bit(root -> right -> code, root -> depth);
//        char_print(&(root -> code[0]));

        DFS(root -> right, code_list, total_bit, header);
    }
    //  printf("-1 ");

    
}



// int *select_sort(int *array, int length) {
//     int index_of_min;
//     int i, j;
//     for (i = 0; i < length - 1; i++) {
//         index_of_min = i;
//         for (j = i+1; j < length; j++) {
//             if (array[j] < array[index_of_min]) {
//                 index_of_min = j;
//             }
//         }
//         if (index_of_min != i) {
//             int temp = array[i];
//             array[i] = array[index_of_min];
//             array[index_of_min] = temp;
//         }
//     }
//     return array;
// }

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


void add_bit(char* bittarray, int k) {
    int i = k / 8;
    int j = k % 8;
    bittarray[i] = ((0x80 >> j) | bittarray[i]);
}

void char_print(char* a, int k) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("%d", !!((a[k] << i) & 0x80));
    }
}

int check_target_binary(char *a, int k) {
    int i = k / 8;
    int j = k % 8;
    return (!!((a[i] << j) & 0x80));
}
