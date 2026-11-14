/* Hides every effect node none of whose points is in view. Returns -1
   when culling is off. */
struct Def088168BC {
    char pad_00[0x20];
    short points;
};

struct Node088168BC {
    char pad_00[0x48];
    Def088168BC *def;
};

struct Cull088168BC {
    unsigned char enabled;
};

extern "C" {

extern void *D_089D4910;

void *func_08899300(void);
void func_0881BAA8(void);
Node088168BC *func_0881CB80(Node088168BC *prev);
void *func_0881E8F4(void *mgr, Def088168BC *def, int point);
int func_088987E8(void *world, void *point);
void func_08819624(Node088168BC *node, int a, int b, int c, int fade, int d, int e);

int func_088168BC(Cull088168BC *cull, int fade)
{
    if (!cull->enabled) {
        return -1;
    }
    Def088168BC *def = 0;
    int i = 0;
    void *world = func_08899300();
    func_0881BAA8();
    void *mgr = D_089D4910;
    Node088168BC *n = 0;
    fade = (unsigned char)(fade != 0);
    while ((n = func_0881CB80(n)) != 0) {
        def = n->def;
        if (def == 0) {
            continue;
        }
        if (def->points == 0) {
            continue;
        }
        for (i = 0; i < def->points; i++) {
            if (func_088987E8(world, func_0881E8F4(mgr, def, i))) {
                break;
            }
        }
        if (i != def->points) {
            continue;
        }
        func_08819624(n, 0, 1, 0, fade, 1, 1);
    }
    return 0;
}

}
