/* Starts a request when downloads are idle and the state exists. */
extern unsigned char D_089D8138;
extern int D_089D8134;
extern void *D_089D8128;
extern int func_088FFB3C(int id, int key, short *out);

int func_088FFAB8(int id, short *out)
{
    if (D_089D8138 != 0) {
        return 0;
    }
    if (D_089D8134 < 0) {
        return 0;
    }
    if (D_089D8128 == 0) {
        return 0;
    }
    func_088FFB3C(id, id, out);
    return 0;
}
