typedef struct Target088953BC {
    float x;
    float y;
    int z;
} Target088953BC;

typedef struct World088953BC {
    unsigned char unknown_000[0x8B0];
    Target088953BC target;
} World088953BC;

typedef struct Camera088953BC {
    unsigned char unknown_00[0x78];
    float x;
    float y;
    int z;
    unsigned char active;
} Camera088953BC;

extern World088953BC *D_089D5EC0;

void func_088953BC(Camera088953BC *self) {
    if (self->active != 0) {
        Target088953BC *target = &D_089D5EC0->target;
        target->x = self->x;
        target->y = self->y;
        target->z = self->z;
    }
}
