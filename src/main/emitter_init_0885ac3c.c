typedef struct Particle {
    char pad_00[0x10];
    float life;
    char pad_14[0x1C];
} Particle;

typedef struct Emitter {
    int handle;
    Particle *items;
    int field_08;
    int field_0C;
    int heap;
} Emitter;

extern Particle *func_0885C6CC(int heap, int size, int a, int b, int c, int d);
extern int func_0899B9F0(int arg);
extern void func_0899BB68(int handle, int a, int b, int c);

void func_0885AC3C(Emitter *self, int heap, int a, int b)
{
    int i;
    int one = 1;

    self->heap = heap;
    self->items = func_0885C6CC(heap, 0x780, 0, 0, one, 0);
    for (i = 0; i < 40; i++) {
        self->items[i].life = 0.0f;
    }
    self->handle = func_0899B9F0(0);
    func_0899BB68(self->handle, a, b, 0);
}
