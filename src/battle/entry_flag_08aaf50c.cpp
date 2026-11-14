typedef struct FlagEntry {
    int field_00;
    int flag;
} FlagEntry;

typedef struct FlagOwner {
    unsigned char active;
} FlagOwner;

extern "C" {

extern int D_08A80218;
extern int func_08A80200(int id);
extern int func_08AAE300(FlagOwner *self);
extern int func_08AAE84C(FlagOwner *self);
extern FlagEntry *func_08AAC7D0(int context, int a, int b);

bool func_08AAF50C(FlagOwner *self)
{
    if (!self->active) {
        return false;
    }
    int context = func_08A80200(D_08A80218);
    int a = func_08AAE300(self);
    int b = func_08AAE84C(self);
    FlagEntry *entry = func_08AAC7D0(context, a, b);
    if (entry == 0) {
        return false;
    }
    return entry->flag == 1;
}

}
