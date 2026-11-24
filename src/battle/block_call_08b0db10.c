/* Forwards a byte at +0x25A8+8, the block address +0x2710 and a short at +0x1F9C to func_08B0E40C. */
typedef struct SubB { char pad_00[8]; signed char b; } SubB;
typedef struct SaveBlk { char pad_00[0x1F9C]; short s; char pad_1F9E[0x60A]; SubB sub; } SaveBlk;
extern SaveBlk D_09AFB8A0;
extern void func_08B0E40C(void *self, int b, void *p, int s);
void func_08B0DB10(void *self)
{
    SubB *sub = &D_09AFB8A0.sub;
    func_08B0E40C(self, sub->b, (char *)&D_09AFB8A0 + 0x2710, D_09AFB8A0.s);
}
