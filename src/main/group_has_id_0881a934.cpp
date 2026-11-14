/* Whether a reference names an id: directly, or (kind 1) through a group
   that lists it. */
struct Group0881A934 {
    int count;
    int *ids;
    int pad_8;
};

struct Ref0881A934 {
    int kind;
    int id;
};

struct Table0881A934 {
    char pad_00[0x10];
    Group0881A934 *groups;
};

extern "C" {

bool func_0881A934(Table0881A934 *self, Ref0881A934 *ref, int id)
{
    if (ref->kind == 1) {
        Group0881A934 *g = &self->groups[ref->id];
        for (int i = 0; i < g->count; i++) {
            if (g->ids[i] == id) {
                return true;
            }
        }
        return false;
    }
    return ref->id == id;
}

}
