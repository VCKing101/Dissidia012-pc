/* Writes the short at table index obj->index when kind is 1, else 0. */
typedef struct Entry { short a; short b; int c; } Entry;
typedef struct Obj { char pad[0xC]; int index; char pad2[4]; unsigned char kind; } Obj;
extern Entry *D_089D77D8;

void func_08ADD7EC(Obj *obj, int *out)
{
    if (obj->kind == 1) {
        *out = D_089D77D8[obj->index].b;
        return;
    }
    *out = 0;
}
