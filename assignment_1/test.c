#include <stdio.h>

void char_print(char* a, int k) {
    int i;
    for (i = 0; i < 8; i++) {
        printf("%d", !!((a[k] << i) & 0x80));
    }
    printf("\n");
};

int test_print(char *a, int k) {
    int i = k / 8;
    int j = k % 8;
    return (!!((a[i] << j) & 0x80));
}

int main(int argc, char const *argv[])
{
    char x[] = {5, 6};
    char_print(x, 1);

    if (test_print(x, 15)) {
        printf("1111\n");
    }
    return 0;
}
