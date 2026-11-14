typedef struct Color08871D6C {
    float r;
    float g;
    float b;
    float a;
} Color08871D6C;

typedef struct Owner08871D6C {
    unsigned char unknown_00[0x20];
    Color08871D6C color;
} Owner08871D6C;

void func_08871D6C(Color08871D6C *out, Owner08871D6C *self) {
    *out = self->color;
}
