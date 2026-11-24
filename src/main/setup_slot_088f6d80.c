/* Sets up slot (a, b) via func_088F6A58(a, b, c, d, 0), then func_08900CBC(D_089D8140, slot + 0x70C, c) and copies byte 0x44 + c of D_089D8140 to the slot's +0xE80, sets +0x1182 = 1. */
extern char *D_089D8140;
extern char *D_089D7F38;
extern void func_088F6A58(int a, int b, int c, int d, int e);
extern void func_08900CBC(char *p, char *q, int c);
void func_088F6D80(int a, int b, int c, int d)
{
    char *g = D_089D8140;
    char *slot = D_089D7F38 + a * 0x6A68 + b * 0x1548 + 0x70C;
    func_088F6A58(a, b, c, d, 0);
    func_08900CBC(g, slot, c);
    slot[0x774] = *(g + 0x44 + c);
    slot[0xA76] = 1;
}
