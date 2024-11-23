# P-box permutation table
P = [
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
]

print("static const struct {")
print("    unsigned char byte_index;    // sbox_out中的字节索引")
print("    unsigned char input_mask;    // 输入位掩码")
print("    unsigned char byte_out;      // right输出字节索引")
print("    unsigned char output_mask;   // 输出位掩码")
print("} P_LOOKUP[32] = {")

for i in range(32):
    # P[i]是1-32的数字，需要转换为0-31
    p_val = P[i] - 1
    
    # 计算输入的字节索引和位掩码
    in_byte = p_val >> 3
    in_bit = p_val & 7
    in_mask = 0x80 >> in_bit
    
    # 计算输出的字节索引和位掩码
    out_byte = i >> 3
    out_bit = i & 7
    out_mask = 0x80 >> out_bit
    
    print(f"    {{{in_byte}, 0x{in_mask:02x}, {out_byte}, 0x{out_mask:02x}}}, // bit {P[i]} -> bit {i+1}")

print("};")