struct Entry { char data_00[0x38]; char name[0x2C]; unsigned char active; char data_65[3]; };
struct Table { Entry entries[45]; };
extern "C" {
int func_0893585C(const char *a, const char *b);

Entry *func_088A59CC(Table *t, const char *name)
{
    for (int i = 0; i < 45; i++) {
        Entry *e = &t->entries[i];
        if (e->active == 1 && func_0893585C(e->name, name) == 0) {
            return e;
        }
    }
    return 0;
}
}
