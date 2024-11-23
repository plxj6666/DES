#include "../inc/des.h"
#include <string.h>
#include <stdint.h>
// Initial Permutation Table
static const unsigned char IP[] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

// Final Permutation Table
static const unsigned char FP[] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

// Expansion Table
// static const unsigned char E[] = {
//     32, 1, 2, 3, 4, 5,
//     4, 5, 6, 7, 8, 9,
//     8, 9, 10, 11, 12, 13,
//     12, 13, 14, 15, 16, 17,
//     16, 17, 18, 19, 20, 21,
//     20, 21, 22, 23, 24, 25,
//     24, 25, 26, 27, 28, 29,
//     28, 29, 30, 31, 32, 1
// };

// // S-boxes
// static const unsigned char S[8][64] = {
//     {
//         14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
//         0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
//         4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
//         15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
//     },
//     {
//         15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
//         3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
//         0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
//         13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
//     },
//     {
//         10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
//         13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
//         13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
//         1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
//     },
//     {
//         7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
//         13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
//         10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
//         3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
//     },
//     {
//         2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
//         14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
//         4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
//         11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
//     },
//     {
//         12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
//         10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
//         9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
//         4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
//     },
//     {
//         4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
//         13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
//         1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
//         6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
//     },
//     {
//         13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
//         1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
//         7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
//         2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
//     }
// };

// P-box permutation
// static const unsigned char P[] = {
//     16, 7, 20, 21, 29, 12, 28, 17,
//     1, 15, 23, 26, 5, 18, 31, 10,
//     2, 8, 24, 14, 32, 27, 3, 9,
//     19, 13, 30, 6, 22, 11, 4, 25
// };

// Key schedule tables
static const unsigned char PC1[] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

static const unsigned char PC2[] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

// Key rotation schedule
static const unsigned char rotations[] = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

// Helper functions
static void rotate_left(unsigned char *data, int len, int count);
static void f_function(unsigned char *right, unsigned char *subkey);

// 使用内联函数替代宏
static inline unsigned char get_bit(const unsigned char *data, int pos) {
    return (data[pos >> 3] >> (7 - (pos & 7))) & 1;
}

static inline void set_bit(unsigned char *data, int pos) {
    data[pos >> 3] |= (0x80 >> (pos & 7));
}

// 轮次宏
#define ROUND(i, lr, subKeys) \
    { \
        unsigned char temp[4]; \
        memcpy(temp, lr + 4, 4); \
        f_function(lr + 4, subKeys[i]); \
        for(int j = 0; j < 4; j++) \
            lr[j + 4] ^= lr[j]; \
        memcpy(lr, temp, 4); \
    }

int des_make_subkeys(const unsigned char key[8], unsigned char subKeys[16][6]) {
    unsigned char pc1m[56];
    unsigned char pcr[56];
    
    // Apply PC1 permutation to key
    for(int i = 0; i < 56; i++) {
        pc1m[i] = (key[(PC1[i] - 1) >> 3] >> (7 - ((PC1[i] - 1) & 7))) & 1;
    }
    
    // Generate 16 subkeys
    for(int round = 0; round < 16; round++) {
        // Copy to temp array
        for(int i = 0; i < 56; i++)
            pcr[i] = pc1m[i];
            
        // Perform left rotations
        rotate_left(pcr, 28, rotations[round]);
        rotate_left(pcr + 28, 28, rotations[round]);
        
        // Update working key
        for(int i = 0; i < 56; i++)
            pc1m[i] = pcr[i];
            
        // Generate subkey through PC2
        memset(subKeys[round], 0, 6);
        for(int i = 0; i < 48; i++) {
            subKeys[round][i/8] |= 
                pcr[PC2[i] - 1] << (7 - (i & 7));
        }
    }
    
    return 0;
}

void des_encrypt_block(const unsigned char *input, 
                      unsigned char subKeys[16][6], 
                      unsigned char *output) {
    // 使用aligned内存以提高访问速度
    unsigned char __attribute__((aligned(16))) block[8] = {0};
    unsigned char __attribute__((aligned(16))) lr[8] = {0};
    
    // 使用register提示编译器
    register int i;
    
    // 初始置换优化：使用查表法
    #pragma GCC unroll 8
    for(i = 0; i < 64; i++) {
        if(input[(IP[i] - 1) >> 3] & (0x80 >> ((IP[i] - 1) & 7)))
            block[i >> 3] |= (0x80 >> (i & 7));
    }
    
    memcpy(lr, block, 8);
    
    // 展开16轮循环以减少循环开销
    #define DES_ROUND(round) \
        ROUND(round, lr, subKeys)
    
    DES_ROUND(0);  DES_ROUND(1);  DES_ROUND(2);  DES_ROUND(3);
    DES_ROUND(4);  DES_ROUND(5);  DES_ROUND(6);  DES_ROUND(7);
    DES_ROUND(8);  DES_ROUND(9);  DES_ROUND(10); DES_ROUND(11);
    DES_ROUND(12); DES_ROUND(13); DES_ROUND(14); DES_ROUND(15);
    
    // 最后交换优化
    #pragma GCC unroll 4
    for (i = 0; i < 4; i++) {
        unsigned char tmp = lr[i];
        lr[i] = lr[i + 4];
        lr[i + 4] = tmp;
    }
    
    // 最终置换优化
    memset(output, 0, 8);
    #pragma GCC unroll 8
    for(i = 0; i < 64; i++) {
        if(lr[(FP[i] - 1) >> 3] & (0x80 >> ((FP[i] - 1) & 7)))
            output[i >> 3] |= (0x80 >> (i & 7));
    }
}

void des_decrypt_block(const unsigned char *input, 
                      unsigned char subKeys[16][6], 
                      unsigned char *output) {
    // Decryption is the same as encryption with reversed subkeys
    unsigned char reversed_subkeys[16][6];
    // for(int i = 0; i < 16; i++) {
    //     memcpy(reversed_subkeys[i], subKeys[15 - i], 6);
    // }
    #define MEM_ROUND(i) memcpy(reversed_subkeys[i], subKeys[15 - i], 6);
    MEM_ROUND(0)
    MEM_ROUND(1)
    MEM_ROUND(2)
    MEM_ROUND(3)
    MEM_ROUND(4)
    MEM_ROUND(5)
    MEM_ROUND(6)
    MEM_ROUND(7)
    MEM_ROUND(8)
    MEM_ROUND(9)
    MEM_ROUND(10)
    MEM_ROUND(11)
    MEM_ROUND(12)
    MEM_ROUND(13)
    MEM_ROUND(14)
    MEM_ROUND(15)
    des_encrypt_block(input, reversed_subkeys, output);
}



static inline void rotate_left(unsigned char *data, int len, int count) {
    unsigned char temp[256];
    memcpy(temp, data, count);
    memmove(data, data + count, len - count);
    memcpy(data + len - count, temp, count);
}

// 预计算S盒结果
static const unsigned char SBOX_RESULT[8][64] = {
{14, 0, 4, 15, 13, 7, 1, 4, 2, 14, 15, 2, 11, 13, 8, 1, 3, 10, 10, 6, 6, 12, 12, 11, 5, 9, 9, 5, 0, 3, 7, 8, 4, 15, 1, 12, 14, 8, 8, 2, 13, 4, 6, 9, 2, 1, 11, 7, 15, 5, 12, 11, 9, 3, 7, 14, 3, 10, 10, 0, 5, 6, 0, 13},
{15, 3, 1, 13, 8, 4, 14, 7, 6, 15, 11, 2, 3, 8, 4, 14, 9, 12, 7, 0, 2, 1, 13, 10, 12, 6, 0, 9, 5, 11, 10, 5, 0, 13, 14, 8, 7, 10, 11, 1, 10, 3, 4, 15, 13, 4, 1, 2, 5, 11, 8, 6, 12, 7, 6, 12, 9, 0, 3, 5, 2, 14, 15, 9},
{10, 13, 0, 7, 9, 0, 14, 9, 6, 3, 3, 4, 15, 6, 5, 10, 1, 2, 13, 8, 12, 5, 7, 14, 11, 12, 4, 11, 2, 15, 8, 1, 13, 1, 6, 10, 4, 13, 9, 0, 8, 6, 15, 9, 3, 8, 0, 7, 11, 4, 1, 15, 2, 14, 12, 3, 5, 11, 10, 5, 14, 2, 7, 12},
{7, 13, 13, 8, 14, 11, 3, 5, 0, 6, 6, 15, 9, 0, 10, 3, 1, 4, 2, 7, 8, 2, 5, 12, 11, 1, 12, 10, 4, 14, 15, 9, 10, 3, 6, 15, 9, 0, 0, 6, 12, 10, 11, 1, 7, 13, 13, 8, 15, 9, 1, 4, 3, 5, 14, 11, 5, 12, 2, 7, 8, 2, 4, 14},
{2, 14, 12, 11, 4, 2, 1, 12, 7, 4, 10, 7, 11, 13, 6, 1, 8, 5, 5, 0, 3, 15, 15, 10, 13, 3, 0, 9, 14, 8, 9, 6, 4, 11, 2, 8, 1, 12, 11, 7, 10, 1, 13, 14, 7, 2, 8, 13, 15, 6, 9, 15, 12, 0, 5, 9, 6, 10, 3, 4, 0, 5, 14, 3},
{12, 10, 1, 15, 10, 4, 15, 2, 9, 7, 2, 12, 6, 9, 8, 5, 0, 6, 13, 1, 3, 13, 4, 14, 14, 0, 7, 11, 5, 3, 11, 8, 9, 4, 14, 3, 15, 2, 5, 12, 2, 9, 8, 5, 12, 15, 3, 10, 7, 11, 0, 14, 4, 1, 10, 7, 1, 6, 13, 0, 11, 8, 6, 13},
{4, 13, 11, 0, 2, 11, 14, 7, 15, 4, 0, 9, 8, 1, 13, 10, 3, 14, 12, 3, 9, 5, 7, 12, 5, 2, 10, 15, 6, 8, 1, 6, 1, 6, 4, 11, 11, 13, 13, 8, 12, 1, 3, 4, 7, 10, 14, 7, 10, 9, 15, 5, 6, 0, 8, 15, 0, 14, 5, 2, 9, 3, 2, 12},
{13, 1, 2, 15, 8, 13, 4, 8, 6, 10, 15, 3, 11, 7, 1, 4, 10, 12, 9, 5, 3, 6, 14, 11, 5, 0, 0, 14, 12, 9, 7, 2, 7, 2, 11, 1, 4, 14, 1, 7, 9, 4, 12, 10, 14, 8, 2, 13, 0, 15, 6, 12, 10, 9, 13, 0, 15, 3, 3, 5, 5, 6, 8, 11}
};

// 优化f_function中的S盒查找
static inline unsigned char sbox_lookup(int box, unsigned char input) {
    return SBOX_RESULT[box][input];
}

static const unsigned char E_expansion_table[] = {
    31, 0, 1, 2, 3, 4, 3, 4,
    5, 6, 7, 8, 7, 8, 9, 10,
    11, 12, 11, 12, 13, 14, 15, 16,
    15, 16, 17, 18, 19, 20, 19, 20,
    21, 22, 23, 24, 23, 24, 25, 26,
    27, 28, 27, 28, 29, 30, 31, 0,
};


static const unsigned char P_permutation_table[] = {
    15, 6, 19, 20, 28, 11, 27, 16,
    0, 14, 22, 25, 4, 17, 30, 9,
    1, 7, 23, 13, 31, 26, 2, 8,
    18, 12, 29, 5, 21, 10, 3, 24,
};

static void f_function(unsigned char *right, unsigned char *subkey) {
    unsigned char expanded[6] = {0};
    unsigned char sbox_out[4] = {0};
    

    // 基于E_expansion_table展开计算
    // 第1字节 (bits 1-8)
    expanded[0] |= (right[E_expansion_table[0] >> 3] & (0x80 >> (E_expansion_table[0] & 7))) ? 0x80 : 0;
    expanded[0] |= (right[E_expansion_table[1] >> 3] & (0x80 >> (E_expansion_table[1] & 7))) ? 0x40 : 0;
    expanded[0] |= (right[E_expansion_table[2] >> 3] & (0x80 >> (E_expansion_table[2] & 7))) ? 0x20 : 0;
    expanded[0] |= (right[E_expansion_table[3] >> 3] & (0x80 >> (E_expansion_table[3] & 7))) ? 0x10 : 0;
    expanded[0] |= (right[E_expansion_table[4] >> 3] & (0x80 >> (E_expansion_table[4] & 7))) ? 0x08 : 0;
    expanded[0] |= (right[E_expansion_table[5] >> 3] & (0x80 >> (E_expansion_table[5] & 7))) ? 0x04 : 0;
    expanded[0] |= (right[E_expansion_table[6] >> 3] & (0x80 >> (E_expansion_table[6] & 7))) ? 0x02 : 0;
    expanded[0] |= (right[E_expansion_table[7] >> 3] & (0x80 >> (E_expansion_table[7] & 7))) ? 0x01 : 0;

    // 第2字节 (bits 9-16)
    expanded[1] |= (right[E_expansion_table[8] >> 3] & (0x80 >> (E_expansion_table[8] & 7))) ? 0x80 : 0;
    expanded[1] |= (right[E_expansion_table[9] >> 3] & (0x80 >> (E_expansion_table[9] & 7))) ? 0x40 : 0;
    expanded[1] |= (right[E_expansion_table[10] >> 3] & (0x80 >> (E_expansion_table[10] & 7))) ? 0x20 : 0;
    expanded[1] |= (right[E_expansion_table[11] >> 3] & (0x80 >> (E_expansion_table[11] & 7))) ? 0x10 : 0;
    expanded[1] |= (right[E_expansion_table[12] >> 3] & (0x80 >> (E_expansion_table[12] & 7))) ? 0x08 : 0;
    expanded[1] |= (right[E_expansion_table[13] >> 3] & (0x80 >> (E_expansion_table[13] & 7))) ? 0x04 : 0;
    expanded[1] |= (right[E_expansion_table[14] >> 3] & (0x80 >> (E_expansion_table[14] & 7))) ? 0x02 : 0;
    expanded[1] |= (right[E_expansion_table[15] >> 3] & (0x80 >> (E_expansion_table[15] & 7))) ? 0x01 : 0;

    // 第3字节 (bits 17-24)
    expanded[2] |= (right[E_expansion_table[16] >> 3] & (0x80 >> (E_expansion_table[16] & 7))) ? 0x80 : 0;
    expanded[2] |= (right[E_expansion_table[17] >> 3] & (0x80 >> (E_expansion_table[17] & 7))) ? 0x40 : 0;
    expanded[2] |= (right[E_expansion_table[18] >> 3] & (0x80 >> (E_expansion_table[18] & 7))) ? 0x20 : 0;
    expanded[2] |= (right[E_expansion_table[19] >> 3] & (0x80 >> (E_expansion_table[19] & 7))) ? 0x10 : 0;
    expanded[2] |= (right[E_expansion_table[20] >> 3] & (0x80 >> (E_expansion_table[20] & 7))) ? 0x08 : 0;
    expanded[2] |= (right[E_expansion_table[21] >> 3] & (0x80 >> (E_expansion_table[21] & 7))) ? 0x04 : 0;
    expanded[2] |= (right[E_expansion_table[22] >> 3] & (0x80 >> (E_expansion_table[22] & 7))) ? 0x02 : 0;
    expanded[2] |= (right[E_expansion_table[23] >> 3] & (0x80 >> (E_expansion_table[23] & 7))) ? 0x01 : 0;

    // 第4字节 (bits 25-32)
    expanded[3] |= (right[E_expansion_table[24] >> 3] & (0x80 >> (E_expansion_table[24] & 7))) ? 0x80 : 0;
    expanded[3] |= (right[E_expansion_table[25] >> 3] & (0x80 >> (E_expansion_table[25] & 7))) ? 0x40 : 0;
    expanded[3] |= (right[E_expansion_table[26] >> 3] & (0x80 >> (E_expansion_table[26] & 7))) ? 0x20 : 0;
    expanded[3] |= (right[E_expansion_table[27] >> 3] & (0x80 >> (E_expansion_table[27] & 7))) ? 0x10 : 0;
    expanded[3] |= (right[E_expansion_table[28] >> 3] & (0x80 >> (E_expansion_table[28] & 7))) ? 0x08 : 0;
    expanded[3] |= (right[E_expansion_table[29] >> 3] & (0x80 >> (E_expansion_table[29] & 7))) ? 0x04 : 0;
    expanded[3] |= (right[E_expansion_table[30] >> 3] & (0x80 >> (E_expansion_table[30] & 7))) ? 0x02 : 0;
    expanded[3] |= (right[E_expansion_table[31] >> 3] & (0x80 >> (E_expansion_table[31] & 7))) ? 0x01 : 0;

    // 第5字节 (bits 33-40)
    expanded[4] |= (right[E_expansion_table[32] >> 3] & (0x80 >> (E_expansion_table[32] & 7))) ? 0x80 : 0;
    expanded[4] |= (right[E_expansion_table[33] >> 3] & (0x80 >> (E_expansion_table[33] & 7))) ? 0x40 : 0;
    expanded[4] |= (right[E_expansion_table[34] >> 3] & (0x80 >> (E_expansion_table[34] & 7))) ? 0x20 : 0;
    expanded[4] |= (right[E_expansion_table[35] >> 3] & (0x80 >> (E_expansion_table[35] & 7))) ? 0x10 : 0;
    expanded[4] |= (right[E_expansion_table[36] >> 3] & (0x80 >> (E_expansion_table[36] & 7))) ? 0x08 : 0;
    expanded[4] |= (right[E_expansion_table[37] >> 3] & (0x80 >> (E_expansion_table[37] & 7))) ? 0x04 : 0;
    expanded[4] |= (right[E_expansion_table[38] >> 3] & (0x80 >> (E_expansion_table[38] & 7))) ? 0x02 : 0;
    expanded[4] |= (right[E_expansion_table[39] >> 3] & (0x80 >> (E_expansion_table[39] & 7))) ? 0x01 : 0;

    // 第6字节 (bits 41-48)
    expanded[5] |= (right[E_expansion_table[40] >> 3] & (0x80 >> (E_expansion_table[40] & 7))) ? 0x80 : 0;
    expanded[5] |= (right[E_expansion_table[41] >> 3] & (0x80 >> (E_expansion_table[41] & 7))) ? 0x40 : 0;
    expanded[5] |= (right[E_expansion_table[42] >> 3] & (0x80 >> (E_expansion_table[42] & 7))) ? 0x20 : 0;
    expanded[5] |= (right[E_expansion_table[43] >> 3] & (0x80 >> (E_expansion_table[43] & 7))) ? 0x10 : 0;
    expanded[5] |= (right[E_expansion_table[44] >> 3] & (0x80 >> (E_expansion_table[44] & 7))) ? 0x08 : 0;
    expanded[5] |= (right[E_expansion_table[45] >> 3] & (0x80 >> (E_expansion_table[45] & 7))) ? 0x04 : 0;
    expanded[5] |= (right[E_expansion_table[46] >> 3] & (0x80 >> (E_expansion_table[46] & 7))) ? 0x02 : 0;
    expanded[5] |= (right[E_expansion_table[47] >> 3] & (0x80 >> (E_expansion_table[47] & 7))) ? 0x01 : 0;

    // XOR with subkey
    for(int i = 0; i < 6; i++) {
        expanded[i] ^= subkey[i];
    }

    // S-box lookup (保持原有代码不变)
    for(int i = 0; i < 8; i++) {
        int block_pos = i * 6;
        unsigned char sixbits;
        
        int byte_index = block_pos / 8;
        int bit_offset = block_pos % 8;
        
        if (bit_offset <= 2) {
            sixbits = (expanded[byte_index] >> (2 - bit_offset)) & 0x3F;
        } else {
            sixbits = ((expanded[byte_index] << (bit_offset - 2)) & 0x3F) |
                      (expanded[byte_index + 1] >> (10 - bit_offset));
        }
        
        unsigned char sbox_val = SBOX_RESULT[i][sixbits];
        sbox_out[i/2] |= (i % 2) ? sbox_val : (sbox_val << 4);
    }
    memset(right, 0, 4);
    
    // P-box permutation
    for(int i = 0; i < 32; i++) {
        if(sbox_out[P_permutation_table[i] >> 3] & (0x80 >> (P_permutation_table[i] & 7)))
            right[i >> 3] |= 0x80 >> (i & 7);
    }
}



