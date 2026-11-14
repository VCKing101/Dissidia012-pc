/* Adds a light entry for an effect, unless a player or boss actor already
   has a lit node of that effect. Returns -1 when lights are off and -2
   when the table is full. */
struct Kind08816BA8 {
    int kind;
};

struct Actor08816BA8 {
    char pad_000[0x3A4];
    Kind08816BA8 *kind;
};

struct Def08816BA8 {
    short id;
};

struct Light08816BA8 {
    char pad_00[0x10];
    void *on;
};

struct Node08816BA8 {
    char pad_00[0x48];
    Def08816BA8 *def;
    Light08816BA8 *light;
};

struct Lights08816BA8 {
    unsigned char enabled;
    char pad_01[3];
    char table[4];
};

extern "C" {

Node08816BA8 *func_0881CB80(Node08816BA8 *prev);
int func_0881B278(void *table, int id, Actor08816BA8 *actor, void *a, void *b);

int func_08816BA8(Lights08816BA8 *self, int id, Actor08816BA8 *actor, void *a, void *b)
{
    if (!self->enabled) {
        return -1;
    }
    if (actor != 0 && (actor->kind->kind == 3 || actor->kind->kind == 0x17)) {
        int found = 0;
        Node08816BA8 *n = 0;
        while ((n = func_0881CB80(n)) != 0) {
            Def08816BA8 *def = n->def;
            if (def == 0) {
                continue;
            }
            if (def->id != id) {
                continue;
            }
            Light08816BA8 *light = n->light;
            if (light == 0) {
                continue;
            }
            if (light->on == 0) {
                continue;
            }
            found = 1;
            break;
        }
        if (found == 1) {
            return 0;
        }
    }
    if (func_0881B278(self->table, id, actor, a, b)) {
        return -2;
    }
    return 0;
}

}
