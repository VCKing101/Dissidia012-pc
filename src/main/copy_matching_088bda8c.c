/* Copies up to max 12-byte records whose first halfword equals id from a table into out, advancing *count. */
typedef struct Rec { short a, b, c, d, e, f; } Rec;
void func_088BDA8C(Rec *out, int max, int *count, Rec *tbl, int id)
{
    int i = 0;
    Rec *p = tbl;
    if (i < *(int *)((char *)tbl + 0x30)) {
        do {
            if (p->a == id) {
                if (*count < max) {
                    Rec *q = &out[*count];
                    *q = *p;
                    *count = *count + 1;
                } else {
                    return;
                }
            }
            i++;
            p++;
        } while (i < *(int *)((char *)tbl + 0x30));
    }
}
