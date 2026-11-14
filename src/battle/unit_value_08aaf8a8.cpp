typedef struct ValueEntry {
    char pad_00[0x14];
    float value;
} ValueEntry;

typedef struct ValueOwner {
    unsigned char active;
} ValueOwner;

extern "C" {

extern int D_08A80218;
extern int func_08A80200(int id);
extern int func_08AAE300(ValueOwner *self);
extern int func_08AAE84C(ValueOwner *self);
extern ValueEntry *func_08AAC7D0(int context, int a, int b);

float func_08AAF8A8(ValueOwner *self)
{
    if (!self->active) {
        return 0.0f;
    }
    int context = func_08A80200(D_08A80218);
    int a = func_08AAE300(self);
    int b = func_08AAE84C(self);
    ValueEntry *entry = func_08AAC7D0(context, a, b);
    if (entry == 0) {
        return 0.0f;
    }
    return entry->value;
}

}
