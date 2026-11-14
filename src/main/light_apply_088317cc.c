typedef struct Actor088317CC {
    unsigned char unknown_000[0x164];
    unsigned char field_164[0x1CC];
    unsigned char field_330[0x64];
    int field_394;
    unsigned char unknown_398[0xF8];
    float x;
    float y;
    float z;
} Actor088317CC;

void func_0886AED0(int handle, float x, float y, float z, void *a, void *b);

void func_088317CC(Actor088317CC *self) {
    func_0886AED0(self->field_394, self->x, self->y, self->z, self->field_330, self->field_164);
}
