struct Entry { char name[0x18]; };
struct Header { int field_00; int field_04; int field_08; int count; };
extern "C" {
extern Header *D_089D72A8;
extern Entry *D_089D72B4;
int func_0893585C(const char *a, const char *b);

Entry *func_088B2410(const char *name)
{
    for (int i = 0; i < D_089D72A8->count; i++) {
        Entry *e = &D_089D72B4[i];
        if (func_0893585C(e->name, name) == 0) {
            return e;
        }
    }
    return 0;
}
}
