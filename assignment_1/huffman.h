//
//  huffman.h
//  assignment_1
//
//  Created by Wujian on 13/8/18.
//  Copyright © 2018 Wujian. All rights reserved.
//


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
} Tree_node;



#endif /* huffman_h */
