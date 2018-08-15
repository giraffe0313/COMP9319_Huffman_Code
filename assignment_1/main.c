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
    list_node *head = construct_linked_list(frequence);
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
    memset(header.bitarray, 0, 1016);

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
    int number_of_bytes_needed = total_bit / 8 + 1;
    if (!number_of_bytes_needed % 8) {
        number_of_bytes_needed--;
    }
    printf("number of bytes is %d\n", number_of_bytes_needed);
    header.number_of_bits = number_of_bytes_needed;

    char *encode_result = malloc(number_of_bytes_needed * sizeof(char));
    
    file = fopen(file_pwd, "r");
    
    int total_number = 0;
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

    // Output_file output_file;
    // output_file.encode_result = encode_result;
    // output_file.header = header;




    file = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/output.header", "wb");
    fwrite(&header, sizeof(Header), 1, file);
    fclose(file);

    file = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/output.header", "a+");
    fseek(file, 0, SEEK_END);
    fwrite(encode_result, sizeof(char), number_of_bytes_needed, file);
    fclose(file);

    // file = fopen("/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/output.file", "wb");
    // fwrite(&output_file, sizeof(Header) + number_of_bytes_needed * sizeof(char), 1, file);

    return 0;
}









