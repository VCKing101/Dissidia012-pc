/* Copies a 0x778-byte record from `src` to `dst` (source first). */
typedef struct Record778 {
    int words[0x778 / 4];
} Record778;

void func_088F8E50(Record778 *src, Record778 *dst)
{
    *dst = *src;
}
