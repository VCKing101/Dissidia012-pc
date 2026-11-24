/* Pushes an exception-scope frame (kind 4) onto the chain D_089DA444 and clears the nine-word record it guards. */
typedef struct EhFrame08993260 { struct EhFrame08993260 *prev; char kind; char pad[3]; int *rec; } EhFrame08993260;
extern EhFrame08993260 *D_089DA444;
void func_08993260(EhFrame08993260 *f, int *rec, int v) {
    char kind = 4;
    f->prev = D_089DA444;
    D_089DA444 = f;
    f->kind = kind;
    f->rec = rec;
    rec[0] = 0; rec[1] = 0; rec[2] = 0; rec[3] = 0;
    rec[4] = v;
    rec[5] = 0; rec[6] = 0; rec[7] = 0; rec[8] = 0;
}
