/* Light table updates through groups and links. */
struct Ref0881A454 {
    int kind;
    int id;
};

struct Link0881A454 {
    Ref0881A454 from;
    Ref0881A454 to;
};

struct Group0881A454 {
    int count;
    int *ids;
    unsigned char value;
};

struct Table0881A454 {
    char pad_00[0xC];
    int group_count;
    Group0881A454 *groups;
    int link_count;
    Link0881A454 **links;
};

extern "C" {

void func_0881A798(Table0881A454 *self, int id, unsigned char a, unsigned char b, unsigned char c);
bool func_0881A934(Table0881A454 *self, Ref0881A454 *ref, int id);
void func_0881A6AC(Table0881A454 *self, Ref0881A454 *ref, unsigned char a, unsigned char b, unsigned char c);

/* Sets the value of every group that contains the id and re-applies it to
   all of the group's members. */
void func_0881A454(Table0881A454 *self, int id, unsigned char a, unsigned char b)
{
    for (int i = 0; i < self->group_count; i++) {
        Group0881A454 *g = &self->groups[i];
        bool found = false;
        for (int j = 0; j < g->count; j++) {
            if (g->ids[j] == id) {
                found = true;
                break;
            }
        }
        if (!found) {
            continue;
        }
        g->value = a;
        for (int k = 0; k < self->groups[i].count; k++) {
            func_0881A798(self, self->groups[i].ids[k], a, b, 1);
        }
    }
}

/* Applies the opposite value to the other end of every link that names
   the id. */
void func_0881A594(Table0881A454 *self, int id, unsigned char a, unsigned char b)
{
    for (int i = 0; i < self->link_count; i++) {
        Link0881A454 *l = self->links[i];
        Ref0881A454 *to = &l->to;
        if (func_0881A934(self, &l->from, id) == true) {
            func_0881A6AC(self, to, !a, b, 0);
        } else if (func_0881A934(self, to, id) == true) {
            func_0881A6AC(self, &l->from, !a, b, 0);
        }
    }
}

/* Applies a value to the ids a reference names. */
void func_0881A6AC(Table0881A454 *self, Ref0881A454 *ref, unsigned char a, unsigned char b, unsigned char c)
{
    if (ref->kind == 1) {
        Group0881A454 *g = &self->groups[ref->id];
        g->value = a;
        for (int i = 0; i < g->count; i++) {
            func_0881A798(self, g->ids[i], a, b, c);
        }
    } else {
        func_0881A798(self, ref->id, a, b, c);
    }
}

}
