/* Stores p in D_089D7F3C; a null p selects the default block at 0x09B69B00 with -1 in its second word. */
extern int *D_089D7F3C;
extern int D_09B69B00[];
void func_088F707C(int *p)
{
    if (p) {
        D_089D7F3C = p;
    } else {
        D_09B69B00[1] = -1;
        D_089D7F3C = D_09B69B00;
    }
}
