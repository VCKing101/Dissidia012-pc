/* Shows the effect nodes of each group a definition lists, or starts the
   group's world effect when no node of it is running. Returns -1 when
   culling is off and -2 without a definition. */
struct GroupDef08816A00 {
    char pad_00[0x1C];
    short groups;
};

struct NodeDef08816A00 {
    short id;
};

struct Node08816A00 {
    char pad_00[0x48];
    NodeDef08816A00 *def;
};

struct Cull08816A00 {
    unsigned char enabled;
};

extern "C" {

extern void *D_089D4910;

int func_0881E898(void *mgr, GroupDef08816A00 *def, int index);
void func_0881BAA8(void);
Node08816A00 *func_0881CB80(Node08816A00 *prev);
void func_08818F00(Node08816A00 *node, int motion, int flags);
void *func_08899300(void);
void func_088981B4(void *world, int id, int a, int b);

int func_08816A00(Cull08816A00 *cull, GroupDef08816A00 *def)
{
    if (!cull->enabled) {
        return -1;
    }
    if (def == 0) {
        return -2;
    }
    void *mgr = D_089D4910;
    for (int i = 0; i < def->groups; i++) {
        int id = func_0881E898(mgr, def, i);
        int found = 0;
        Node08816A00 *n = 0;
        while (func_0881BAA8(), (n = func_0881CB80(n)) != 0) {
            NodeDef08816A00 *d = n->def;
            if (d == 0) {
                continue;
            }
            if (d->id != id) {
                continue;
            }
            func_08818F00(n, 0x3F2, 0);
            found = 1;
        }
        if (!found) {
            func_088981B4(func_08899300(), id, 1, 1);
        }
    }
    return 0;
}

}
