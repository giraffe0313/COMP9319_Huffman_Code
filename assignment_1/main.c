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
// #include "linked_list.h"
#include "huffman.h"

#define TRUE 1     
#define FALSE 0

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
    list_node *head = construct_linked_list(frequence);
    // list_print(head);
    
    // construct huffman tree
    Tree_node *root = construct_huffman(head);

    // with pointer points the corresponding tree node
    struct code_information* code_list = malloc(128 * sizeof(struct code_information));
    memset(code_list, 0, 128 * sizeof(struct code_information));

    int total_bit = 0;  // the number of encode bit

    // header with 1024 bytes
    Header header;
    header.length = 0;
    memset(header.bitarray, 0, 1016);

    printf("Encode: header tree information:\n");
    DFS(root, code_list, &total_bit, &header);
    printf("\n");

    printf("Encode: each char encode result:\n");
    for (i = 0; i < 128; i++) {
        if (code_list[i].exist_or_not) {
            printf("%d code is ", i);
            char_print(code_list[i].code -> code, 0);
            printf("\n");
        }
    }
    
    // printf("%d\n", root ->position);
    // printf("total bit is %d\n", total_bit);
    
    
//    start encode
    int number_of_bytes_needed = total_bit / 8 + 1;
    if (!number_of_bytes_needed % 8) {
        number_of_bytes_needed--;
    }
    // printf("number of bytes is %d\n", number_of_bytes_needed);
    
    
    
    header.number_of_bits = total_bit;
    // header.length = number_of_bytes_needed;

    char *encode_result = malloc(number_of_bytes_needed * sizeof(char));
    
    file = fopen(file_pwd, "r");
    
    int total_number = 0;
    while ((get_result = fgetc(file)) != EOF) {
        if (!code_list[get_result].exist_or_not) {
            printf("bug!!!!!!]\n");
        }
        for (i = 0; i < code_list[get_result].code -> depth; i++) {
            // printf("debug print: get_result is %d, depth is %d, code is ", get_result, code_list[get_result].code -> depth);
            // char_print(code_list[get_result].code -> code, 0);
            // printf("\n");
            if (check_target_binary(code_list[get_result].code -> code, i)) {
                add_bit(encode_result, total_number);
            }
            total_number++;
        }
    }
    fclose(file);
    
    printf("Encode: encode result is:\n");
    for (int i = 0; i < total_bit; i++) {
        // char_print(encode_result, i/8);
        if (check_target_binary(encode_result, i)) {
            printf("1 ");
        } else {
            printf("0 ");
        }
    }
    printf("\n");
    // Output_file output_file;
    // output_file.encode_result = encode_result;
    // output_file.header = header;

    
    

    file = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/output.file", "wb");
    fwrite(&header, sizeof(Header), 1, file);
    fclose(file);

    file = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/output.file", "a+");
    fseek(file, 0, SEEK_END);
    fwrite(encode_result, sizeof(char), number_of_bytes_needed, file);
    fclose(file);

    printf("Encode: header length is %d, number of bits is %d\n", header.length, header.number_of_bits);
    // file = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/output.file", "wb");
    // fwrite(&output_file, sizeof(Header) + number_of_bytes_needed * sizeof(char), 1, file);
    printf("\n\n\n");




    /* 
    * start decode
    */

    file = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/output.file", "r");
    int input[2] = {0};
    fread(input, sizeof(int), 2, file);
    printf("Decode: input int 1 is %d, input int 2 is %d\n", input[0], input[1]);
    
    // int header_need_bytes = input[0] / 8 + 1;
    char *header_information = malloc(input[0] * sizeof(char));
    memset(header_information, 0, input[0] * sizeof(char));
    fread(header_information, sizeof(char), input[0], file);
    
    // create tree root
    Tree_node *recover_root = malloc(sizeof(Tree_node));
    Tree_node_init(recover_root, -1, 0);
    Tree_node *temp_root = recover_root;

    int interator = 0;
    int should_be_right = FALSE; 
    
    // input[0] is header length of bytes, input[1] is encode length of bits;
    
    printf("Decode: construct tree: \n");
    while (interator < input[0]) {
        Tree_node *new_node = malloc(sizeof(Tree_node));
        if (header_information[interator] == 0 && header_information[interator+1] == 1) {
            Tree_node_init(new_node, header_information[interator+2], 0);
            printf("node is %d\n", header_information[interator+2]);
            if (should_be_right) {
                temp_root -> right = new_node;
            } else {
                temp_root -> left = new_node;
            }
            new_node -> parent = temp_root;
            temp_root = new_node;
            temp_root = back_trace(temp_root);
            should_be_right = TRUE;
            interator = interator + 3;
        } else {
            Tree_node_init(new_node, -1, 0);
            if (should_be_right) {
                temp_root -> right = new_node;
            } else {
                temp_root -> left = new_node;
            }
            new_node -> parent = temp_root;
            should_be_right = FALSE;
            temp_root = new_node;
            interator++;
        }
    }
    if (temp_root != recover_root) {
        printf("bug!!!!!!\n");
    } 

    printf("Decode: recovery result is: \n");
    DFS(recover_root, code_list, &total_bit, &header);
    printf("\n");

    

    /* decode */
    FILE *file_recovery = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/recover.txt", "w");
    

    int number_of_bits = input[1];
    int bit_index = 0;
    int loop_indicator;
    char recover_char;
    temp_root = recover_root;
    fseek(file, 1024, SEEK_SET);
    printf("Decode: recover code result:\n");
    while ((get_result = fgetc(file)) != EOF) {
        char true_result = (char) get_result;
        bit_index = 0;
        loop_indicator = (number_of_bits <= 8) ? number_of_bits : 8;
        while (bit_index < loop_indicator) {
            if (check_target_binary(&true_result, bit_index)) {
                temp_root = temp_root -> right;
                printf("1 ");
            } else {
                temp_root = temp_root -> left;
                printf("0 ");
            }
            if (!temp_root -> left && !temp_root -> right) {
                recover_char = temp_root -> val;
                printf("value: %d ", temp_root -> val);
                fwrite(&recover_char, sizeof(char), 1, file_recovery);
                temp_root = recover_root;
            }
            bit_index++;
        }
        number_of_bits = number_of_bits - 8;
    }
    printf("\n");
    
    
    
    



    return 0;
}









