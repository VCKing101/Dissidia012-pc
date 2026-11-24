/* Collects up to five active entries from sixteen into a record, or fills the record with -1 when none are active. */
typedef struct Rec { signed char a; short b; short c[5]; } Rec;
extern int func_08A5C764(char *);
extern int func_08A5C7C8(char *);
extern int func_08A5C7D0(char *);
void func_08A5D9D4(char *owner, Rec *rec) {
    int n = 0;
    int i = 0;
    char *it = owner + 0xA8;
    Rec *w = rec;
    Rec *r = rec;
    do {
        if (func_08A5C764(it)) {
            r->a = func_08A5C7C8(it);
            r->b = 0;
            w->c[0] = func_08A5C7D0(it);
            n++;
            w = (Rec *)((char *)w + 2);
            if (n >= 5) break;
        }
        i++;
        it += 0x54;
    } while (i < 0x10);
    if (n == 0) {
        int j;
        r->a = -1;
        r->b = -1;
        j = 0;
        do {
            r->c[0] = -1;
            j++;
            r = (Rec *)((char *)r + 2);
        } while (j < 5);
    } else {
        while (n < 5) {
            w->c[0] = -1;
            n++;
            w = (Rec *)((char *)w + 2);
        }
    }
}
