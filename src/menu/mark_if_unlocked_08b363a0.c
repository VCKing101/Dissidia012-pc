/* When flag is set and func_0892379C(D_09B6DB74) is zero, sets the byte at 0x09B6DCDC. */
struct Blk { char pad[0xB4]; void *h; char pad2[0x21C - 0xB8]; unsigned char mark; };
extern struct Blk D_09B6DAC0;
extern int func_0892379C(void *h);
void func_08B363A0(int unused, unsigned char flag)
{
    if (flag) {
        struct Blk *b = &D_09B6DAC0;
        if (func_0892379C(b->h) == 0) { b->mark = 1; }
    }
}
