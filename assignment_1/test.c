#include <stdio.h>
#include <string.h>

void char_print(char* a, int k) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("%d", !!((a[k] << i) & 0x80));
    }
    printf("\n");
};

// void char_print_all(char* a, int k) {
//     int i = k / 8;
//     for (int x = 0; x < i; x++) {

//     }
// }

int test_print(char *a, int k) {
    int i = k / 8;
    int j = k % 8;
    return (!!((a[i] << j) & 0x80));
}

int main(int argc, char const *argv[])
{
    // char x[] = {5, 6};
    // char_print(x, 1);

    // if (test_print(x, 15)) {
    //     printf("1111\n");
    // }
    char *x;
    int a = 5;
    // int b = 6;
    // sprintf(x, "%d", a);
    // itoa(a, x, 10);
    x = (char *) a;
    for (int i = 0; i < 32; i++) {
        if (test_print(x, i)) {
            printf("1 "); 
        } else {
            printf("0 ");
        }
        
    }
    printf("\n");
    return 0;
}
