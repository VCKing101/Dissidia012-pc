// Copies three Vec4s within entry `row` (0xB0 bytes each, base +0x110) to +0x40/0x50/0x60 later, copies a float and fills a unit table.
extern void func_089A3868(void *dst, void *src);
typedef struct Row57 {
    char pad0[0x30];
    float f30;
    char pad1[0x3C];
    float f70;
    char pad2[0xC];
    float f80;
    float f84;
    float f88;
    float f8C;
    float f90;
    float f94;
    float f98;
    float f9C;
    unsigned char fA0;
} Row57;
void func_08B3C040(char *base, int row)
{
    Row57 *r = (Row57 *)(base + row * 0xB0 + 0x110);
    func_089A3868((char *)r + 0x40, (char *)r);
    func_089A3868((char *)r + 0x50, (char *)r + 0x10);
    func_089A3868((char *)r + 0x60, (char *)r + 0x20);
    r->f70 = r->f30;
    r->f80 = 1.0f;
    r->f84 = 0.0f;
    r->f88 = 1.0f;
    r->f8C = 0.0f;
    r->f90 = 1.0f;
    r->f94 = 0.0f;
    r->f98 = 1.0f;
    r->f9C = 0.0f;
    r->fA0 = 1;
}
