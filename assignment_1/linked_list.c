//
//  linked_list.c
//  assignment_1
//
//  Created by Wujian on 10/8/18.
//  Copyright © 2018 Wujian. All rights reserved.
//

#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

list_node *list_insert_node(list_node *head, int name, int frequency, Tree_node *tree_node) {
    if (!head -> frequency) {
        head -> name = name;
        head -> frequency = frequency;
        head -> next = NULL;
        return head;
    }
    
    list_node *temp = head;
    list_node *new_node = malloc(sizeof(list_node*));
    new_node -> name = name;
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
