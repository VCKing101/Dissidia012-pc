/* Fills 15 byte pairs from a rotating table: entry (i + start) wrapped by the count; the first pair's first byte is the stored default, later ones the previous entry's second byte. */
typedef struct B088F25D8 { char pad[0x40]; unsigned char sel; unsigned char start; unsigned char def; } B088F25D8;
typedef struct E088F25D8 { unsigned char a; unsigned char b; unsigned char pad[2]; } E088F25D8;
extern char *D_089D712C;
int func_088C6AA0(unsigned char sel);
E088F25D8 *func_088C6AF4(unsigned char sel);
void func_088F25D8(unsigned char *out0)
{
    unsigned char *out = out0;
    B088F25D8 *g = (B088F25D8 *)(D_089D712C + 0x1DE98);
    unsigned char start = g->start;
    unsigned char def = g->def;
    int n = func_088C6AA0(g->sel);
    E088F25D8 *t = func_088C6AF4(g->sel);
    int i;
    for (i = 0; i < 0xF; i++, out += 2) {
        int k = i + start;
        unsigned char first;
        if (n <= k) {
            k -= n;
        }
        if (i != 0) {
            first = t[k].b;
        } else {
            first = def;
        }
        out[0] = first;
        out[1] = t[k].a;
    }
}
