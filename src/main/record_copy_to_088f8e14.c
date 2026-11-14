/* Copies a 0x778-byte record from `src` to `dst`. */
typedef struct Record778 {
    int words[0x778 / 4];
} Record778;

void func_088F8E14(Record778 *dst, Record778 *src)
{
    *dst = *src;
}
