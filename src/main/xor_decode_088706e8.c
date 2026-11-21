/* Decode n bytes: each output byte is key4[r1 % 4] ^ key8[r2 % 8] ^ r3 for the next three values of
 * the MSVC-style generator (x * 0x15A4E35 + 1, bits 16..30) seeded with 0xDDFF. */
void func_088706E8(const unsigned char *key4, const unsigned char *key8, unsigned char *dst, int n)
{
    int i;
    unsigned int seed = 0xDDFF;

    for (i = 0; i < n; i++) {
        int a = (seed * 0x15A4E35 + 1) >> 16 & 0x7FFF;
        int b = ((unsigned int)a * 0x15A4E35 + 1) >> 16 & 0x7FFF;

        seed = ((unsigned int)b * 0x15A4E35 + 1) >> 16 & 0x7FFF;
        dst[i] = key4[a % 4] ^ key8[b % 8] ^ seed;
    }
}
