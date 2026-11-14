/* Builds a hit group: on first use collects the actor's parts of the
   group's kind, then fills an entry for each part the actor can use now. */
struct Part0883E79C {
    unsigned char id;
};

struct Entry0883E79C {
    char data[0x1C];
};

struct Actor0883E79C;

struct Group0883E79C {
    Actor0883E79C *actor;
    short count;
    unsigned char kind;
    unsigned char total;
    Part0883E79C **parts;
    Entry0883E79C entries[1];
};

extern "C" {

Part0883E79C *func_08835134(Actor0883E79C *actor, int kind, Part0883E79C *prev);
void *func_08842100(int size);
int func_08837D18(Actor0883E79C *actor, int part);
void func_08812664(Entry0883E79C *entry, Actor0883E79C *actor, Part0883E79C *part);

void func_0883E79C(Group0883E79C *g)
{
    if (g->parts == 0) {
        Part0883E79C *p = 0;
        int n = 0;
        while ((p = func_08835134(g->actor, g->kind, p)) != 0) {
            n++;
        }
        g->total = n;
        if (0 < n) {
            g->parts = (Part0883E79C **)func_08842100(g->total * 4);
            p = 0;
            int i = 0;
            while ((p = func_08835134(g->actor, g->kind, p)) != 0) {
                g->parts[i] = p;
                i++;
            }
        }
    }
    g->count = 0;
    for (int i = 0; i < g->total; i++) {
        if (func_08837D18(g->actor, g->parts[i]->id)) {
            func_08812664(&g->entries[g->count], g->actor, g->parts[i]);
            g->count++;
        }
    }
}

}
