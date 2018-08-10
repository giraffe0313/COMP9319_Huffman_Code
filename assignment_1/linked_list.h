//
//  linked_list.h
//  assignment_1
//
//  Created by Wujian on 10/8/18.
//  Copyright © 2018 Wujian. All rights reserved.
//

#ifndef linked_list_h
#define linked_list_h

typedef struct _Tree_node {
    int frequenct;
    int val;
    struct _Tree_node *left;
    struct _Tree_node *right;
} Tree_node;

typedef struct _list_node {
    int name;
    int frequency;
    Tree_node *tree_node;
    struct _list_node *next;
} list_node;



list_node *list_insert_node(list_node *head, int name, int frequency, Tree_node *tree_node);

void list_print(list_node *head);

void list_clean(list_node *head);


#endif /* linked_list_h */


