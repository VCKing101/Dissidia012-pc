/* Steps a light entry's fade-in: sets the scene light's range from the
   fade time until the fade is done. Returns -1 for an unused entry and -2
   without a renderer. */
struct Light08819F84 {
    char data[0xC];
};

struct Gfx08819F84 {
    char data[0x10];
};

struct Renderer08819F84 {
    char pad_000[0x8B0];
    Gfx08819F84 gfx;
};

struct EntryBits08819F84 {
    unsigned int used : 1;
    signed int slot : 16;
    signed int state : 2;
    unsigned int pad : 13;
};

struct Entry08819F84 {
    union {
        unsigned int flags;
        EntryBits08819F84 bits;
    };
    float time;
};

extern "C" {

extern float D_089D5EAC;
extern float D_089C7508;
extern Renderer08819F84 *D_089D5EC0;

void func_08864C1C(Light08819F84 *l);
float func_08864CB8(Gfx08819F84 *g);
float func_08864CC0(Gfx08819F84 *g);
void func_08864C74(Light08819F84 *l, float near_range);
void func_08864C7C(Light08819F84 *l, float far_range);
void func_08864C84(Light08819F84 *l, int r, int g, int b);
void *func_08899300(void);
void func_08898464(void *world, int slot, Light08819F84 *l);
void func_08864C48(Light08819F84 *l, int mode);
void func_0881A124(Entry08819F84 *e);

int func_08819F84(Entry08819F84 *e)
{
    if (!(e->flags & 1)) {
        return -1;
    }
    if (e->bits.state == 0) {
        e->time += D_089D5EAC;
        if (e->time < D_089C7508) {
            Gfx08819F84 *gfx = &D_089D5EC0->gfx;
            if (gfx == 0) {
                return -2;
            }
            Light08819F84 light;
            func_08864C1C(&light);
            func_08864C74(&light, func_08864CB8(gfx) * e->time / D_089C7508);
            func_08864C7C(&light, func_08864CC0(gfx) * e->time / D_089C7508);
            func_08864C84(&light, 0xFF, 0xFF, 0xFF);
            func_08898464(func_08899300(), e->bits.slot, &light);
            func_08864C48(&light, 2);
        } else {
            func_0881A124(e);
        }
    }
    return 0;
}

}
