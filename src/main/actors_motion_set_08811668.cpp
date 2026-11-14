/* When the scene allows it, starts motion `id` on every actor of the
   16-slot actor table that can play it. */
struct Motion08811668 {
    char data[1];
};

struct Actor08811668 {
    char pad_000[0x130];
    Motion08811668 motion;
    char pad_131[0x540 - 0x131];
    short motion_id;
};

struct SceneInfo08811668 {
    char pad_0[6];
    unsigned short flags;
};

struct World08811668 {
    char pad_000[0xD64];
    char actors[1];
};

struct Scene08811668 {
    World08811668 *world;
    SceneInfo08811668 *info;
};

extern "C" {

Actor08811668 *func_0884F6B8(char *table, int index);
int func_0882F6D0(Motion08811668 *motion, int id);
void func_0882FA10(Motion08811668 *motion, int id, int mode, float time, float start);

void func_08811668(Scene08811668 *self, int id, float time)
{
    if (!(self->info->flags & 2)) {
        return;
    }
    char *actors = self->world->actors;
    for (int i = 0; i < 16; i++) {
        Actor08811668 *a = func_0884F6B8(actors, i);
        Motion08811668 *m = &a->motion;
        if (a != 0) {
            if (func_0882F6D0(m, id)) {
                func_0882FA10(m, id, 0, time, 0.0f);
                a->motion_id = id;
            }
        }
    }
}

}
