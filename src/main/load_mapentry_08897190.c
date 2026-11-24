/* Loads the "mapentry.bin" record from the archive into the static buffer when it fits and hands it to the parser. */
typedef struct Rec { int pad; int data; unsigned size; } Rec;
extern int *D_089D6C10;
extern char *D_089D6C14;
extern char D_09B49220[];
extern Rec *func_08869AB4(int arc, char *name);
extern void func_08935524(void *dst, int src, unsigned n);
extern void func_08897124(int *tbl, void *buf);
void func_08897190(int arc)
{
    Rec *r;
    if (arc == 0) {
        return;
    }
    if (*D_089D6C10 != 0) {
        return;
    }
    r = func_08869AB4(arc, D_089D6C14);
    if (r == 0) {
        return;
    }
    if (r->size < 0x3CC5) {
        func_08935524(D_09B49220, r->data, r->size);
        func_08897124(D_089D6C10, D_09B49220);
    }
}
