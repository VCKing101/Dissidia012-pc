/* Runs func_088F9A7C on the block at D_09AFB8A0, then sets the state byte 0x25B0 bytes into it to 8. */
typedef struct Sub088F9B40 { unsigned char pad[8]; unsigned char state; } Sub088F9B40;
typedef struct Block088F9B40 { unsigned char pad[0x25A8]; Sub088F9B40 sub; } Block088F9B40;
extern Block088F9B40 D_09AFB8A0;
extern void func_088F9A7C(Block088F9B40 *b);
void func_088F9B40(void)
{
    Sub088F9B40 *s = &D_09AFB8A0.sub;
    func_088F9A7C(&D_09AFB8A0);
    s->state = 8;
}
