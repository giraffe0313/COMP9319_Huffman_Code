//
//  main.c
//  assignment_1
//
//  Created by Wujian on 10/8/18.
//  Copyright © 2018 Wujian. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

int *select_sort(int *array, int length);

Tree_node* Tree_node_init(Tree_node* new, int val, int frequency) {
    new -> val = val;
    new -> frequenct = frequency;
    new -> left = NULL;
    new -> right = NULL;
    return new;
}


int main(int argc, const char * argv[]) {
    
    const char file_pwd[] = "/Users/wujian/Downloads/CSE_Course/18s2/COMP9319/assignment_1/assignment_1/test1.txt";
    FILE *file = fopen(file_pwd, "r");
    int get_result;
    int distinct_num = 0;
    int frequence[127] = {0};
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
    head -> frequency = 0;
    
    // counting frequency and construct linked list
    for (int i = 1; i < 127; i++) {
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
    
    
    
    
    
    
    
    return 0;
}

int *select_sort(int *array, int length) {
    int index_of_min;
    for (int i = 0; i < length - 1; i++) {
        index_of_min = i;
        for (int j = i+1; j < length; j++) {
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
