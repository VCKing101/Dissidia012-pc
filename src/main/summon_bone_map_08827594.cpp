/* Builds a summon's bone map: for each of its bones, the owner's bone of
   the same name (-1 where the owner has none). */
struct Skeleton08827594 {
    char pad_00[0x14];
    char **names;
    short count;
};

struct Body08827594 {
    char pad_00[0x2C];
    Skeleton08827594 *skeleton;
};

struct Actor08827594 {
    char pad_000[0x390];
    Body08827594 *body;
};

struct BonePair08827594 {
    short bone;
    short owner_bone;
};

struct Summon08827594 {
    char pad_000[0x390];
    Body08827594 *body;
    char pad_394[0x550 - 0x394];
    Actor08827594 *owner;
    char pad_554[0x594 - 0x554];
    BonePair08827594 *map;
    int map_count;
};

extern "C" {

void *func_08842100(int size);
int func_08935970(const char *a, const char *b);

void func_08827594(Summon08827594 *self)
{
    Skeleton08827594 *own = self->body->skeleton;
    Skeleton08827594 *owner = self->owner->body->skeleton;
    int count = own->count;
    self->map_count = count;
    if (count <= 0) {
        return;
    }
    self->map = (BonePair08827594 *)func_08842100(count * 4);
    for (int i = 0; i < self->map_count; i++) {
        BonePair08827594 *p = &self->map[i];
        p->bone = -1;
        p->owner_bone = -1;
    }
    int owner_count = owner->count;
    for (int j = 0; j < owner_count; j++) {
        for (int k = 0; k < self->map_count; k++) {
            if (self->map[k].bone >= 0) {
                continue;
            }
            if (func_08935970(own->names[k], owner->names[j]) == 0) {
                self->map[k].bone = k;
                self->map[k].owner_bone = j;
                break;
            }
        }
    }
}

}
