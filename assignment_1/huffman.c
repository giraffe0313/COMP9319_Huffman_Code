//
//  huffman.c
//  assignment_1
//
//  Created by Wujian on 13/8/18.
//  Copyright © 2018 Wujian. All rights reserved.
//

#include "huffman.h"
#define TRUE 1     
#define FALSE 0

Tree_node* Tree_node_init(Tree_node* new, int val, int frequency) {
    new -> val = val;
    new -> frequency = frequency;
    new -> left = NULL;
    new -> right = NULL;
    new -> parent = NULL;
    new -> depth = 0;
    memset(new -> code, 0, 16);
    new -> position = 0;
    return new;
}

list_node *construct_linked_list(int *frequence) {
    int i;
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
    return head;
}


Tree_node *construct_huffman(list_node *head) {
    list_node *list_temp_head = head;
    Tree_node *tree_node_temp = malloc(sizeof(Tree_node));
    tree_node_temp = Tree_node_init(tree_node_temp, -1, 0);
    Tree_node *root = tree_node_temp;
    while (list_temp_head) {
        // printf("list frequency name is %d, value is %d\n",list_temp_head -> name, list_temp_head ->frequency);
        
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

Tree_node *back_trace(Tree_node *node) {
    while (node -> parent) {
        node = node -> parent;
        if (!node -> right) {
            break;
        }
    }
    return node;
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





list_node *list_insert_node(list_node *head, int name, int frequency, Tree_node *tree_node) {
    if (!head -> frequency) {
        head -> name = name;
        head -> frequency = frequency;
        head -> next = NULL;
        head -> tree_node = tree_node;
        return head;
    }
    list_node *temp = head;
    list_node *new_node = malloc(sizeof(list_node));
    assert(new_node);
    new_node -> name = name;
//    printf("name is %d\n", name);
    new_node -> frequency = frequency;
    new_node -> tree_node = tree_node;
    new_node -> next = NULL;
    int flag = 0;
    while (temp -> next != NULL) {
        if (frequency < temp -> next -> frequency) {
            list_node *next_node = temp -> next;
            temp -> next = new_node;
            new_node -> next = next_node;
            flag = 1;
            break;
        }
        temp = temp -> next;
    }
    if (!flag) {
        temp -> next = new_node;
    }
    return head;
}

void list_print(list_node *head) {
    while (head) {
        printf("char is %c, frequency is %d\n", head -> name, head -> frequency);
        head = head -> next;
    }
}


void list_clean(list_node *head) {
    while (head) {
        list_node *next_node = head -> next;
        free(head);
        head = next_node;
    }
}


int get_one_decode_result(FILE* file, int *number_of_bits, int *bit_index, int *loop_indicator, int *get_result, Tree_node *recover_root) {
    char true_result = (char) *get_result;
    Tree_node* temp_root = recover_root;
    while (1) {
        if (!(*bit_index < *loop_indicator)) {
            // if (minus_flag) {
            //     *number_of_bits = *number_of_bits - 8;
            // }
            if ((*get_result = fgetc(file)) != EOF) {
                true_result = (char) *get_result;
                *loop_indicator = (*number_of_bits <= 8) ? *number_of_bits : 8;
                *bit_index = 0;
                *number_of_bits = *number_of_bits - 8;
            } else {
                return -1;
            }
        }
        // minus_flag = TRUE;
        while (*bit_index < *loop_indicator) {
            if (check_target_binary(&true_result, *bit_index)) {
                temp_root = temp_root -> right;
                // printf("1 ");
            } else {
                temp_root = temp_root -> left;
                // printf("0 ");
            }
            if (!temp_root -> left && !temp_root -> right) {
                
                // printf("value: %d ", temp_root -> val);
                // fwrite(&recover_char, sizeof(char), 1, file_recovery);
                
                *bit_index = *bit_index + 1;
                return (int)temp_root -> val;
                
            }
            *bit_index = *bit_index + 1;
        }
    }
    return -1;
}

void prefix_table(char pattern[], int prefix[], int n) {
    prefix[0] = 0;
    int len = 0;
    int i = 1;
    while(i < n) {
        if (pattern[i] == pattern[len]) {
            len++;
            prefix[i] = len;
            i++;
        } else {
            if (len > 0) {
                len = prefix[len - 1];
            } else {
                prefix[i] = len;
                i++;
            }
        }
    }
    for (i = n - 1; i > 0; i--) {
        prefix[i] = prefix[i - 1];
    }
    prefix[0] = -1;
}

