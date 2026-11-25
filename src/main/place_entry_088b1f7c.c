/* Looks up the entry id for the selected row of the page and, when its record belongs to this owner, converts it and hands the result to the placer; returns the placed value. */
typedef struct Rec { char p0[6]; short key; char p1[0x12 - 8]; signed char owner; } Rec;
typedef struct S { char p0[4]; int row; int owner; } S;
typedef struct Page { char p0[0x10]; char tab[0x773 - 0x10]; signed char page; } Page;
extern Rec *func_088B29D8(int id);
extern void func_088C005C(void *dst, int key);
extern void func_088BD6D0(int a, int b, int *out, void *tmp);
int func_088B1F7C(S *self, int a, int b, Page *pg)
{
    int out[1];
    char tmp[0x34];
    int id;
    Rec *r;
    int pgn = pg->page;
    char *t = (char *)pg + 0x10;
    id = *(short *)(t + pgn * 198 + 0xA2 + self->row * 2);
    out[0] = 0;
    if (id != -1) {
        r = func_088B29D8(id);
        if (r != 0) {
            if (r->owner == self->owner) {
                func_088C005C(tmp, r->key);
                func_088BD6D0(a, b, out, tmp);
            }
        }
    }
    return out[0];
}
