//
// Created by yuvallevy on 09/12/2019.
//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int is_powerOfTwo(int number);

int expOfTwo(int number);


int main(int argc, char *argv[]) {
    int k = strtol(argv[1], NULL, 10);

    FILE *file;
    file = fopen("kefel.s", "w");
    if (file == NULL) {
        printf("Unable to create file.\n");
    }
    fprintf(file, ".section .text\n");
    fprintf(file, ".globl  kefel\n");
    fprintf(file, "kefel:");

    if (k == 0) {
        fprintf(file, "movl $0, %%eax\n");
    } else if (k == 1) {
        fprintf(file, "movl %%edi, %%eax\n");
    } else if (k == -1) {
        fprintf(file, "movl %%edi, %%eax\n");
        fprintf(file, "neg, %%eax\n");
    } else if (k == 3) {
        fprintf(file, "leal(%%rdi,3), %%eax\n");
    } else if (k == 5) {
        fprintf(file, "leal(%%rdi,5), %%eax\n");
    } else if (k == 9) {
        fprintf(file, "leal(%%rdi,9), %%eax\n");
    } else if (k % 3 == 0 && is_powerOfTwo(k / 3) == 1) {
        int exp = expOfTwo(k / 3);
        // x = x+x*2
        fprintf(file, "leal (%%edi, %%edi, 2), %%eax\n");
        fprintf(file, "shl $%d, %%eax", exp);
    } else if (k % 5 == 0 && is_powerOfTwo(k / 5) == 1) {
        int exp = expOfTwo(k / 5);
        // x = x+x*2
        fprintf(file, "leal (%%edi, %%edi, 4), %%eax\n");
        fprintf(file, "shl $%d, %%eax", exp);
    } else if (k % 9 == 0 && is_powerOfTwo(k / 9) == 1) {
        int exp = expOfTwo(k / 9);
        // x = x+x*2
        fprintf(file, "leal (%%edi, %%edi, 8), %%eax\n");
        fprintf(file, "shl $%d, %%eax", exp);
    } else {
        int isNeg = 0;
        if (k < 0) {
            k = k * -1;
            isNeg = 1;
        }
        int i, n = 0, m = 0, start = 0, mask = 1,eax = 0;
        for (i = 0; i < 32; i++) {
            if (start == 1) {
                while (start == 1) {
                    if ((mask & k) != 0) {
                        n = i;
                        i++;
                    } else {
                        start = 0;
                        if (n == m) {
                            int j;
                            // (x << n) + (x << n-1) ...
                            for (j = n; j > 0; j--) {
                                if (eax == 0) {
                                    fprintf(file, "movl %%edi, %%eax\n");
                                    fprintf(file, "shl $%d, %%eax\n", j);
                                    eax = 1;
                                } else {
                                    if (isNeg == 1) {
                                        fprintf(file, "movl %%edi, %%ecx\n");
                                        fprintf(file, "shl $%d, %%ecx\n", j);
                                        fprintf(file, "subl %%ecx,%%eax\n");
                                    } else {
                                        fprintf(file, "movl %%edi, %%ecx\n");
                                        fprintf(file, "shl $%d, %%ecx\n", j);
                                        fprintf(file, "addl %%ecx,%%eax\n");
                                    }
                                }
                            }
                        } else {
                            if (eax == 0) {
                                if (isNeg == 1) {
                                    // (x << m ) - (x << n+1)
                                    fprintf(file, "movl %%edi, %%eax\n");
                                    fprintf(file, "shl $%d, %%eax\n", m);
                                    fprintf(file, "movl %%edi, %%ecx\n");
                                    fprintf(file, "shl $%d, %%ecx\n", n + 1);
                                    fprintf(file, "subl %%ecx,%%eax\n");
                                } else {
                                    // (x << n+1 ) - (x << m)
                                    fprintf(file, "movl %%edi, %%eax\n");
                                    fprintf(file, "shl $%d, %%eax\n", n + 1);
                                    fprintf(file, "movl %%edi, %%ecx\n");
                                    fprintf(file, "shl $%d, %%ecx\n", m);
                                    fprintf(file, "subl %%ecx,%%eax\n");
                                }
                                eax = 1;
                            }
                            else {
                                if (isNeg == 1) {
                                    // (x << m ) - (x << n+1)
                                    fprintf(file, "movl %%edi, %%ecx\n");
                                    fprintf(file, "shl $%d, %%ecx\n", m);
                                    fprintf(file, "subl %%ecx,%%eax\n");
                                    fprintf(file, "movl %%edi, %%edx\n");
                                    fprintf(file, "shl $%d, %%edx\n", n+1);
                                    fprintf(file, "subl %%edx,%%eax\n");
                                } else {
                                    // (x << n+1 ) - (x << m)
                                    fprintf(file, "movl %%edi, %%ecx\n");
                                    fprintf(file, "shl $%d, %%ecx\n", n+1);
                                    fprintf(file, "addl %%ecx,%%eax\n");
                                    fprintf(file, "movl %%edi, %%edx\n");
                                    fprintf(file, "shl $%d, %%edx\n", m);
                                    fprintf(file, "subl %%edx,%%eax\n");
                                }
                            }
                        }
                    }
                    mask = mask << 1;
                }
            } else {
                if ((k & mask) != 0) {
                    m = i;
                    n = i;
                    start = 1;
                }
                mask = mask << 1;
            }
        }
    }
    fprintf(file, "ret\n");
    fclose(file);
    return 0;
}


int is_powerOfTwo(int number) {
    if (number == 0) {
        return 0;
    }
    while (number != 1) {
        if (number % 2 != 0) {
            return 0;
        }
        number = number / 2;
    }
    return 1;
}

int expOfTwo(int number) {
    int b = 1;
    int counter = 0;
    while (b < number) {
        b = b << 1;
        counter++;
    }
    return counter;
}