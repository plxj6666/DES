def generate_lookup_table(permutation_table, name):
    print(f"// {name} Lookup Table")
    print("static const struct {")
    print("    unsigned char byte_index;    // 输入字节索引")
    print("    unsigned char bit_mask;      // 输入位掩码")
    print("    unsigned char out_byte;      // 输出字节索引")
    print("    unsigned char out_mask;      // 输出位掩码")
    print(f"}} {name}_LOOKUP[64] = {{")
    
    for i in range(64):
        # 计算源位置
        src_pos = permutation_table[i] - 1  # 转换为0-based索引
        src_byte = src_pos >> 3
        src_bit = src_pos & 7
        src_mask = 0x80 >> src_bit
        
        # 计算目标位置
        dst_byte = i >> 3
        dst_bit = i & 7
        dst_mask = 0x80 >> dst_bit
        
        print(f"    {{{src_byte}, 0x{src_mask:02x}, {dst_byte}, 0x{dst_mask:02x}}}, // {name}[{i}] = {permutation_table[i]}")
    
    print("};")
    print()

# 初始置换表 IP
IP = [
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
]

# 最终置换表 FP
FP = [
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
]

generate_lookup_table(IP, "IP")
generate_lookup_table(FP, "FP")