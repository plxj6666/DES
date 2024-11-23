#include "../inc/des.h"
#include <string.h>
#include <stdint.h>
// Initial Permutation Table
// static const unsigned char IP[] = {
//     58, 50, 42, 34, 26, 18, 10, 2,
//     60, 52, 44, 36, 28, 20, 12, 4,
//     62, 54, 46, 38, 30, 22, 14, 6,
//     64, 56, 48, 40, 32, 24, 16, 8,
//     57, 49, 41, 33, 25, 17, 9, 1,
//     59, 51, 43, 35, 27, 19, 11, 3,
//     61, 53, 45, 37, 29, 21, 13, 5,
//     63, 55, 47, 39, 31, 23, 15, 7
// };

// // Final Permutation Table
// static const unsigned char FP[] = {
//     40, 8, 48, 16, 56, 24, 64, 32,
//     39, 7, 47, 15, 55, 23, 63, 31,
//     38, 6, 46, 14, 54, 22, 62, 30,
//     37, 5, 45, 13, 53, 21, 61, 29,
//     36, 4, 44, 12, 52, 20, 60, 28,
//     35, 3, 43, 11, 51, 19, 59, 27,
//     34, 2, 42, 10, 50, 18, 58, 26,
//     33, 1, 41, 9, 49, 17, 57, 25
// };

// IP Lookup Table
static const struct {
    unsigned char byte_index;    // 输入字节索引
    unsigned char bit_mask;      // 输入位掩码
    unsigned char out_byte;      // 输出字节索引
    unsigned char out_mask;      // 输出位掩码
} IP_LOOKUP[64] = {
    {7, 0x40, 0, 0x80}, // IP[0] = 58
    {6, 0x40, 0, 0x40}, // IP[1] = 50
    {5, 0x40, 0, 0x20}, // IP[2] = 42
    {4, 0x40, 0, 0x10}, // IP[3] = 34
    {3, 0x40, 0, 0x08}, // IP[4] = 26
    {2, 0x40, 0, 0x04}, // IP[5] = 18
    {1, 0x40, 0, 0x02}, // IP[6] = 10
    {0, 0x40, 0, 0x01}, // IP[7] = 2
    {7, 0x10, 1, 0x80}, // IP[8] = 60
    {6, 0x10, 1, 0x40}, // IP[9] = 52
    {5, 0x10, 1, 0x20}, // IP[10] = 44
    {4, 0x10, 1, 0x10}, // IP[11] = 36
    {3, 0x10, 1, 0x08}, // IP[12] = 28
    {2, 0x10, 1, 0x04}, // IP[13] = 20
    {1, 0x10, 1, 0x02}, // IP[14] = 12
    {0, 0x10, 1, 0x01}, // IP[15] = 4
    {7, 0x04, 2, 0x80}, // IP[16] = 62
    {6, 0x04, 2, 0x40}, // IP[17] = 54
    {5, 0x04, 2, 0x20}, // IP[18] = 46
    {4, 0x04, 2, 0x10}, // IP[19] = 38
    {3, 0x04, 2, 0x08}, // IP[20] = 30
    {2, 0x04, 2, 0x04}, // IP[21] = 22
    {1, 0x04, 2, 0x02}, // IP[22] = 14
    {0, 0x04, 2, 0x01}, // IP[23] = 6
    {7, 0x01, 3, 0x80}, // IP[24] = 64
    {6, 0x01, 3, 0x40}, // IP[25] = 56
    {5, 0x01, 3, 0x20}, // IP[26] = 48
    {4, 0x01, 3, 0x10}, // IP[27] = 40
    {3, 0x01, 3, 0x08}, // IP[28] = 32
    {2, 0x01, 3, 0x04}, // IP[29] = 24
    {1, 0x01, 3, 0x02}, // IP[30] = 16
    {0, 0x01, 3, 0x01}, // IP[31] = 8
    {7, 0x80, 4, 0x80}, // IP[32] = 57
    {6, 0x80, 4, 0x40}, // IP[33] = 49
    {5, 0x80, 4, 0x20}, // IP[34] = 41
    {4, 0x80, 4, 0x10}, // IP[35] = 33
    {3, 0x80, 4, 0x08}, // IP[36] = 25
    {2, 0x80, 4, 0x04}, // IP[37] = 17
    {1, 0x80, 4, 0x02}, // IP[38] = 9
    {0, 0x80, 4, 0x01}, // IP[39] = 1
    {7, 0x20, 5, 0x80}, // IP[40] = 59
    {6, 0x20, 5, 0x40}, // IP[41] = 51
    {5, 0x20, 5, 0x20}, // IP[42] = 43
    {4, 0x20, 5, 0x10}, // IP[43] = 35
    {3, 0x20, 5, 0x08}, // IP[44] = 27
    {2, 0x20, 5, 0x04}, // IP[45] = 19
    {1, 0x20, 5, 0x02}, // IP[46] = 11
    {0, 0x20, 5, 0x01}, // IP[47] = 3
    {7, 0x08, 6, 0x80}, // IP[48] = 61
    {6, 0x08, 6, 0x40}, // IP[49] = 53
    {5, 0x08, 6, 0x20}, // IP[50] = 45
    {4, 0x08, 6, 0x10}, // IP[51] = 37
    {3, 0x08, 6, 0x08}, // IP[52] = 29
    {2, 0x08, 6, 0x04}, // IP[53] = 21
    {1, 0x08, 6, 0x02}, // IP[54] = 13
    {0, 0x08, 6, 0x01}, // IP[55] = 5
    {7, 0x02, 7, 0x80}, // IP[56] = 63
    {6, 0x02, 7, 0x40}, // IP[57] = 55
    {5, 0x02, 7, 0x20}, // IP[58] = 47
    {4, 0x02, 7, 0x10}, // IP[59] = 39
    {3, 0x02, 7, 0x08}, // IP[60] = 31
    {2, 0x02, 7, 0x04}, // IP[61] = 23
    {1, 0x02, 7, 0x02}, // IP[62] = 15
    {0, 0x02, 7, 0x01}, // IP[63] = 7
};

// FP Lookup Table
static const struct {
    unsigned char byte_index;    // 输入字节索引
    unsigned char bit_mask;      // 输入位掩码
    unsigned char out_byte;      // 输出字节索引
    unsigned char out_mask;      // 输出位掩码
} FP_LOOKUP[64] = {
    {4, 0x01, 0, 0x80}, // FP[0] = 40
    {0, 0x01, 0, 0x40}, // FP[1] = 8
    {5, 0x01, 0, 0x20}, // FP[2] = 48
    {1, 0x01, 0, 0x10}, // FP[3] = 16
    {6, 0x01, 0, 0x08}, // FP[4] = 56
    {2, 0x01, 0, 0x04}, // FP[5] = 24
    {7, 0x01, 0, 0x02}, // FP[6] = 64
    {3, 0x01, 0, 0x01}, // FP[7] = 32
    {4, 0x02, 1, 0x80}, // FP[8] = 39
    {0, 0x02, 1, 0x40}, // FP[9] = 7
    {5, 0x02, 1, 0x20}, // FP[10] = 47
    {1, 0x02, 1, 0x10}, // FP[11] = 15
    {6, 0x02, 1, 0x08}, // FP[12] = 55
    {2, 0x02, 1, 0x04}, // FP[13] = 23
    {7, 0x02, 1, 0x02}, // FP[14] = 63
    {3, 0x02, 1, 0x01}, // FP[15] = 31
    {4, 0x04, 2, 0x80}, // FP[16] = 38
    {0, 0x04, 2, 0x40}, // FP[17] = 6
    {5, 0x04, 2, 0x20}, // FP[18] = 46
    {1, 0x04, 2, 0x10}, // FP[19] = 14
    {6, 0x04, 2, 0x08}, // FP[20] = 54
    {2, 0x04, 2, 0x04}, // FP[21] = 22
    {7, 0x04, 2, 0x02}, // FP[22] = 62
    {3, 0x04, 2, 0x01}, // FP[23] = 30
    {4, 0x08, 3, 0x80}, // FP[24] = 37
    {0, 0x08, 3, 0x40}, // FP[25] = 5
    {5, 0x08, 3, 0x20}, // FP[26] = 45
    {1, 0x08, 3, 0x10}, // FP[27] = 13
    {6, 0x08, 3, 0x08}, // FP[28] = 53
    {2, 0x08, 3, 0x04}, // FP[29] = 21
    {7, 0x08, 3, 0x02}, // FP[30] = 61
    {3, 0x08, 3, 0x01}, // FP[31] = 29
    {4, 0x10, 4, 0x80}, // FP[32] = 36
    {0, 0x10, 4, 0x40}, // FP[33] = 4
    {5, 0x10, 4, 0x20}, // FP[34] = 44
    {1, 0x10, 4, 0x10}, // FP[35] = 12
    {6, 0x10, 4, 0x08}, // FP[36] = 52
    {2, 0x10, 4, 0x04}, // FP[37] = 20
    {7, 0x10, 4, 0x02}, // FP[38] = 60
    {3, 0x10, 4, 0x01}, // FP[39] = 28
    {4, 0x20, 5, 0x80}, // FP[40] = 35
    {0, 0x20, 5, 0x40}, // FP[41] = 3
    {5, 0x20, 5, 0x20}, // FP[42] = 43
    {1, 0x20, 5, 0x10}, // FP[43] = 11
    {6, 0x20, 5, 0x08}, // FP[44] = 51
    {2, 0x20, 5, 0x04}, // FP[45] = 19
    {7, 0x20, 5, 0x02}, // FP[46] = 59
    {3, 0x20, 5, 0x01}, // FP[47] = 27
    {4, 0x40, 6, 0x80}, // FP[48] = 34
    {0, 0x40, 6, 0x40}, // FP[49] = 2
    {5, 0x40, 6, 0x20}, // FP[50] = 42
    {1, 0x40, 6, 0x10}, // FP[51] = 10
    {6, 0x40, 6, 0x08}, // FP[52] = 50
    {2, 0x40, 6, 0x04}, // FP[53] = 18
    {7, 0x40, 6, 0x02}, // FP[54] = 58
    {3, 0x40, 6, 0x01}, // FP[55] = 26
    {4, 0x80, 7, 0x80}, // FP[56] = 33
    {0, 0x80, 7, 0x40}, // FP[57] = 1
    {5, 0x80, 7, 0x20}, // FP[58] = 41
    {1, 0x80, 7, 0x10}, // FP[59] = 9
    {6, 0x80, 7, 0x08}, // FP[60] = 49
    {2, 0x80, 7, 0x04}, // FP[61] = 17
    {7, 0x80, 7, 0x02}, // FP[62] = 57
    {3, 0x80, 7, 0x01}, // FP[63] = 25
};

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
#define ROUND(i, lr, subKeys) { \
    unsigned char temp[4] __attribute__((aligned(16))); \
    memcpy(temp, lr + 4, 4); \
    f_function(lr + 4, subKeys[i]); \
    lr[4] ^= lr[0]; \
    lr[5] ^= lr[1]; \
    lr[6] ^= lr[2]; \
    lr[7] ^= lr[3]; \
    memcpy(lr, temp, 4); \
}

int des_make_subkeys(const unsigned char key[8], unsigned char subKeys[16][6]) {
    unsigned char __attribute__((aligned(16))) pc1m[56];
    unsigned char __attribute__((aligned(16))) pcr[56];
    
    // Apply PC1 permutation to key - 展开56次循环
    #define PC1_ROUND(i) \
        pc1m[i] = (key[(PC1[i] - 1) >> 3] >> (7 - ((PC1[i] - 1) & 7))) & 1;

    PC1_ROUND(0);  PC1_ROUND(1);  PC1_ROUND(2);  PC1_ROUND(3);  PC1_ROUND(4);  PC1_ROUND(5);  PC1_ROUND(6);  PC1_ROUND(7);
    PC1_ROUND(8);  PC1_ROUND(9);  PC1_ROUND(10); PC1_ROUND(11); PC1_ROUND(12); PC1_ROUND(13); PC1_ROUND(14); PC1_ROUND(15);
    PC1_ROUND(16); PC1_ROUND(17); PC1_ROUND(18); PC1_ROUND(19); PC1_ROUND(20); PC1_ROUND(21); PC1_ROUND(22); PC1_ROUND(23);
    PC1_ROUND(24); PC1_ROUND(25); PC1_ROUND(26); PC1_ROUND(27); PC1_ROUND(28); PC1_ROUND(29); PC1_ROUND(30); PC1_ROUND(31);
    PC1_ROUND(32); PC1_ROUND(33); PC1_ROUND(34); PC1_ROUND(35); PC1_ROUND(36); PC1_ROUND(37); PC1_ROUND(38); PC1_ROUND(39);
    PC1_ROUND(40); PC1_ROUND(41); PC1_ROUND(42); PC1_ROUND(43); PC1_ROUND(44); PC1_ROUND(45); PC1_ROUND(46); PC1_ROUND(47);
    PC1_ROUND(48); PC1_ROUND(49); PC1_ROUND(50); PC1_ROUND(51); PC1_ROUND(52); PC1_ROUND(53); PC1_ROUND(54); PC1_ROUND(55);
    
    // Generate 16 subkeys - 展开16轮循环
    #define SUBKEY_ROUND(round) { \
        /* Copy to temp array */ \
        memcpy(pcr, pc1m, 56); \
        \
        /* Perform left rotations */ \
        rotate_left(pcr, 28, rotations[round]); \
        rotate_left(pcr + 28, 28, rotations[round]); \
        \
        /* Update working key */ \
        memcpy(pc1m, pcr, 56); \
        \
        /* Generate subkey through PC2 */ \
        memset(subKeys[round], 0, 6); \
        /* 展开48次PC2循环 */ \
        _PC2_ROUND(round, 0);  _PC2_ROUND(round, 1);  _PC2_ROUND(round, 2);  _PC2_ROUND(round, 3); \
        _PC2_ROUND(round, 4);  _PC2_ROUND(round, 5);  _PC2_ROUND(round, 6);  _PC2_ROUND(round, 7); \
        _PC2_ROUND(round, 8);  _PC2_ROUND(round, 9);  _PC2_ROUND(round, 10); _PC2_ROUND(round, 11); \
        _PC2_ROUND(round, 12); _PC2_ROUND(round, 13); _PC2_ROUND(round, 14); _PC2_ROUND(round, 15); \
        _PC2_ROUND(round, 16); _PC2_ROUND(round, 17); _PC2_ROUND(round, 18); _PC2_ROUND(round, 19); \
        _PC2_ROUND(round, 20); _PC2_ROUND(round, 21); _PC2_ROUND(round, 22); _PC2_ROUND(round, 23); \
        _PC2_ROUND(round, 24); _PC2_ROUND(round, 25); _PC2_ROUND(round, 26); _PC2_ROUND(round, 27); \
        _PC2_ROUND(round, 28); _PC2_ROUND(round, 29); _PC2_ROUND(round, 30); _PC2_ROUND(round, 31); \
        _PC2_ROUND(round, 32); _PC2_ROUND(round, 33); _PC2_ROUND(round, 34); _PC2_ROUND(round, 35); \
        _PC2_ROUND(round, 36); _PC2_ROUND(round, 37); _PC2_ROUND(round, 38); _PC2_ROUND(round, 39); \
        _PC2_ROUND(round, 40); _PC2_ROUND(round, 41); _PC2_ROUND(round, 42); _PC2_ROUND(round, 43); \
        _PC2_ROUND(round, 44); _PC2_ROUND(round, 45); _PC2_ROUND(round, 46); _PC2_ROUND(round, 47); \
    }

    // PC2循环的展开宏
    #define _PC2_ROUND(round, i) \
        subKeys[round][i/8] |= pcr[PC2[i] - 1] << (7 - (i & 7));

    // 展开16轮子密钥生成
    SUBKEY_ROUND(0);
    SUBKEY_ROUND(1);
    SUBKEY_ROUND(2);
    SUBKEY_ROUND(3);
    SUBKEY_ROUND(4);
    SUBKEY_ROUND(5);
    SUBKEY_ROUND(6);
    SUBKEY_ROUND(7);
    SUBKEY_ROUND(8);
    SUBKEY_ROUND(9);
    SUBKEY_ROUND(10);
    SUBKEY_ROUND(11);
    SUBKEY_ROUND(12);
    SUBKEY_ROUND(13);
    SUBKEY_ROUND(14);
    SUBKEY_ROUND(15);
    
    return 0;
}

void des_encrypt_block(const unsigned char *input, 
                      unsigned char subKeys[16][6], 
                      unsigned char *output) {
    unsigned char __attribute__((aligned(16))) block[8] = {0};
    unsigned char __attribute__((aligned(16))) lr[8] = {0};
    
    // 优化的初始置换
    #define IP_ROUND(i) \
        if(input[IP_LOOKUP[i].byte_index] & IP_LOOKUP[i].bit_mask) \
            block[IP_LOOKUP[i].out_byte] |= IP_LOOKUP[i].out_mask;

    // 使用循环+展开混合方式
    for(int i = 0; i < 64; i += 8) {
        IP_ROUND(i+0); IP_ROUND(i+1); IP_ROUND(i+2); IP_ROUND(i+3);
        IP_ROUND(i+4); IP_ROUND(i+5); IP_ROUND(i+6); IP_ROUND(i+7);
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
    for (int i = 0; i < 4; i++) {
        unsigned char tmp = lr[i];
        lr[i] = lr[i + 4];
        lr[i + 4] = tmp;
    }
    
    // 优化的最终置换
    memset(output, 0, 8);
    #define FP_ROUND(i) \
        if(lr[FP_LOOKUP[i].byte_index] & FP_LOOKUP[i].bit_mask) \
            output[FP_LOOKUP[i].out_byte] |= FP_LOOKUP[i].out_mask;

    FP_ROUND(0);  FP_ROUND(1);  FP_ROUND(2);  FP_ROUND(3);
    FP_ROUND(4);  FP_ROUND(5);  FP_ROUND(6);  FP_ROUND(7);
    FP_ROUND(8);  FP_ROUND(9);  FP_ROUND(10); FP_ROUND(11);
    FP_ROUND(12); FP_ROUND(13); FP_ROUND(14); FP_ROUND(15);
    FP_ROUND(16); FP_ROUND(17); FP_ROUND(18); FP_ROUND(19);
    FP_ROUND(20); FP_ROUND(21); FP_ROUND(22); FP_ROUND(23);
    FP_ROUND(24); FP_ROUND(25); FP_ROUND(26); FP_ROUND(27);
    FP_ROUND(28); FP_ROUND(29); FP_ROUND(30); FP_ROUND(31);
    FP_ROUND(32); FP_ROUND(33); FP_ROUND(34); FP_ROUND(35);
    FP_ROUND(36); FP_ROUND(37); FP_ROUND(38); FP_ROUND(39);
    FP_ROUND(40); FP_ROUND(41); FP_ROUND(42); FP_ROUND(43);
    FP_ROUND(44); FP_ROUND(45); FP_ROUND(46); FP_ROUND(47);
    FP_ROUND(48); FP_ROUND(49); FP_ROUND(50); FP_ROUND(51);
    FP_ROUND(52); FP_ROUND(53); FP_ROUND(54); FP_ROUND(55);
    FP_ROUND(56); FP_ROUND(57); FP_ROUND(58); FP_ROUND(59);
    FP_ROUND(60); FP_ROUND(61); FP_ROUND(62); FP_ROUND(63);
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

static const struct {
    unsigned char byte_index;  // 预计算好的字节索引
    unsigned char mask;        // 预计算好的位掩码
    unsigned char output_mask; // 预计算好的输出掩码
} E_LOOKUP[48] = {
    // 第1字节 (bits 1-8)
    {3, 0x01, 0x80}, // bit 32
    {0, 0x80, 0x40}, // bit 1
    {0, 0x40, 0x20}, // bit 2
    {0, 0x20, 0x10}, // bit 3
    {0, 0x10, 0x08}, // bit 4
    {0, 0x08, 0x04}, // bit 5
    {0, 0x10, 0x02}, // bit 4
    {0, 0x08, 0x01}, // bit 5

    // 第2字节 (bits 9-16)
    {0, 0x04, 0x80}, // bit 6
    {0, 0x02, 0x40}, // bit 7
    {0, 0x01, 0x20}, // bit 8
    {1, 0x80, 0x10}, // bit 9
    {0, 0x01, 0x08}, // bit 8
    {1, 0x80, 0x04}, // bit 9
    {1, 0x40, 0x02}, // bit 10
    {1, 0x20, 0x01}, // bit 11

    // 第3字节 (bits 17-24)
    {1, 0x10, 0x80}, // bit 12
    {1, 0x08, 0x40}, // bit 13
    {1, 0x10, 0x20}, // bit 12
    {1, 0x08, 0x10}, // bit 13
    {1, 0x04, 0x08}, // bit 14
    {1, 0x02, 0x04}, // bit 15
    {1, 0x01, 0x02}, // bit 16
    {2, 0x80, 0x01}, // bit 17

    // 第4字节 (bits 25-32)
    {1, 0x01, 0x80}, // bit 16
    {2, 0x80, 0x40}, // bit 17
    {2, 0x40, 0x20}, // bit 18
    {2, 0x20, 0x10}, // bit 19
    {2, 0x10, 0x08}, // bit 20
    {2, 0x08, 0x04}, // bit 21
    {2, 0x10, 0x02}, // bit 20
    {2, 0x08, 0x01}, // bit 21

    // 第5字节 (bits 33-40)
    {2, 0x04, 0x80}, // bit 22
    {2, 0x02, 0x40}, // bit 23
    {2, 0x01, 0x20}, // bit 24
    {3, 0x80, 0x10}, // bit 25
    {2, 0x01, 0x08}, // bit 24
    {3, 0x80, 0x04}, // bit 25
    {3, 0x40, 0x02}, // bit 26
    {3, 0x20, 0x01}, // bit 27

    // 第6字节 (bits 41-48)
    {3, 0x10, 0x80}, // bit 28
    {3, 0x08, 0x40}, // bit 29
    {3, 0x10, 0x20}, // bit 28
    {3, 0x08, 0x10}, // bit 29
    {3, 0x04, 0x08}, // bit 30
    {3, 0x02, 0x04}, // bit 31
    {3, 0x01, 0x02}, // bit 32
    {0, 0x80, 0x01}  // bit 1
};

static const struct {
    unsigned char byte_index;    // sbox_out中的字节索引
    unsigned char input_mask;    //          
    unsigned char byte_out;      // right输出字节索引
    unsigned char output_mask;   // 输出位掩码
} P_LOOKUP[32] = {
    {1, 0x01, 0, 0x80}, // bit 16 -> bit 1
    {0, 0x02, 0, 0x40}, // bit 7 -> bit 2
    {2, 0x10, 0, 0x20}, // bit 20 -> bit 3
    {2, 0x08, 0, 0x10}, // bit 21 -> bit 4
    {3, 0x08, 0, 0x08}, // bit 29 -> bit 5
    {1, 0x10, 0, 0x04}, // bit 12 -> bit 6
    {3, 0x10, 0, 0x02}, // bit 28 -> bit 7
    {2, 0x80, 0, 0x01}, // bit 17 -> bit 8
    {0, 0x80, 1, 0x80}, // bit 1 -> bit 9
    {1, 0x02, 1, 0x40}, // bit 15 -> bit 10
    {2, 0x02, 1, 0x20}, // bit 23 -> bit 11
    {3, 0x40, 1, 0x10}, // bit 26 -> bit 12
    {0, 0x08, 1, 0x08}, // bit 5 -> bit 13
    {2, 0x40, 1, 0x04}, // bit 18 -> bit 14
    {3, 0x02, 1, 0x02}, // bit 31 -> bit 15
    {1, 0x40, 1, 0x01}, // bit 10 -> bit 16
    {0, 0x40, 2, 0x80}, // bit 2 -> bit 17
    {0, 0x01, 2, 0x40}, // bit 8 -> bit 18
    {2, 0x01, 2, 0x20}, // bit 24 -> bit 19
    {1, 0x04, 2, 0x10}, // bit 14 -> bit 20
    {3, 0x01, 2, 0x08}, // bit 32 -> bit 21
    {3, 0x20, 2, 0x04}, // bit 27 -> bit 22
    {0, 0x20, 2, 0x02}, // bit 3 -> bit 23
    {1, 0x80, 2, 0x01}, // bit 9 -> bit 24
    {2, 0x20, 3, 0x80}, // bit 19 -> bit 25
    {1, 0x08, 3, 0x40}, // bit 13 -> bit 26
    {3, 0x04, 3, 0x20}, // bit 30 -> bit 27
    {0, 0x04, 3, 0x10}, // bit 6 -> bit 28
    {2, 0x04, 3, 0x08}, // bit 22 -> bit 29
    {1, 0x20, 3, 0x04}, // bit 11 -> bit 30
    {0, 0x10, 3, 0x02}, // bit 4 -> bit 31
    {3, 0x80, 3, 0x01}, // bit 25 -> bit 32
};

static void f_function(unsigned char *right, unsigned char *subkey) {
    unsigned char __attribute__((aligned(16))) expanded[6] = {0};
    unsigned char __attribute__((aligned(16))) sbox_out[4] = {0};
    
    // 使用预计算表进行扩展置换 - 展开循环
    #define E_LOOKUP_ROUND(i) \
        if(right[E_LOOKUP[i].byte_index] & E_LOOKUP[i].mask) \
            expanded[i/8] |= E_LOOKUP[i].output_mask;

    // 替换原来的for循环
    E_LOOKUP_ROUND(0);  E_LOOKUP_ROUND(1);  E_LOOKUP_ROUND(2);  E_LOOKUP_ROUND(3);
    E_LOOKUP_ROUND(4);  E_LOOKUP_ROUND(5);  E_LOOKUP_ROUND(6);  E_LOOKUP_ROUND(7);
    E_LOOKUP_ROUND(8);  E_LOOKUP_ROUND(9);  E_LOOKUP_ROUND(10); E_LOOKUP_ROUND(11);
    E_LOOKUP_ROUND(12); E_LOOKUP_ROUND(13); E_LOOKUP_ROUND(14); E_LOOKUP_ROUND(15);
    E_LOOKUP_ROUND(16); E_LOOKUP_ROUND(17); E_LOOKUP_ROUND(18); E_LOOKUP_ROUND(19);
    E_LOOKUP_ROUND(20); E_LOOKUP_ROUND(21); E_LOOKUP_ROUND(22); E_LOOKUP_ROUND(23);
    E_LOOKUP_ROUND(24); E_LOOKUP_ROUND(25); E_LOOKUP_ROUND(26); E_LOOKUP_ROUND(27);
    E_LOOKUP_ROUND(28); E_LOOKUP_ROUND(29); E_LOOKUP_ROUND(30); E_LOOKUP_ROUND(31);
    E_LOOKUP_ROUND(32); E_LOOKUP_ROUND(33); E_LOOKUP_ROUND(34); E_LOOKUP_ROUND(35);
    E_LOOKUP_ROUND(36); E_LOOKUP_ROUND(37); E_LOOKUP_ROUND(38); E_LOOKUP_ROUND(39);
    E_LOOKUP_ROUND(40); E_LOOKUP_ROUND(41); E_LOOKUP_ROUND(42); E_LOOKUP_ROUND(43);
    E_LOOKUP_ROUND(44); E_LOOKUP_ROUND(45); E_LOOKUP_ROUND(46); E_LOOKUP_ROUND(47);

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
    
    // P-box permutation - 展开循环
    #define P_LOOKUP_ROUND(i) \
        if(sbox_out[P_LOOKUP[i].byte_index] & P_LOOKUP[i].input_mask) \
            right[P_LOOKUP[i].byte_out] |= P_LOOKUP[i].output_mask;

    P_LOOKUP_ROUND(0);  P_LOOKUP_ROUND(1);  P_LOOKUP_ROUND(2);  P_LOOKUP_ROUND(3);
    P_LOOKUP_ROUND(4);  P_LOOKUP_ROUND(5);  P_LOOKUP_ROUND(6);  P_LOOKUP_ROUND(7);
    P_LOOKUP_ROUND(8);  P_LOOKUP_ROUND(9);  P_LOOKUP_ROUND(10); P_LOOKUP_ROUND(11);
    P_LOOKUP_ROUND(12); P_LOOKUP_ROUND(13); P_LOOKUP_ROUND(14); P_LOOKUP_ROUND(15);
    P_LOOKUP_ROUND(16); P_LOOKUP_ROUND(17); P_LOOKUP_ROUND(18); P_LOOKUP_ROUND(19);
    P_LOOKUP_ROUND(20); P_LOOKUP_ROUND(21); P_LOOKUP_ROUND(22); P_LOOKUP_ROUND(23);
    P_LOOKUP_ROUND(24); P_LOOKUP_ROUND(25); P_LOOKUP_ROUND(26); P_LOOKUP_ROUND(27);
    P_LOOKUP_ROUND(28); P_LOOKUP_ROUND(29); P_LOOKUP_ROUND(30); P_LOOKUP_ROUND(31);
}
