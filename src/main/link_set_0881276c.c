typedef struct Source0881276C {
    unsigned char unknown_00[5];
    unsigned char kind;
    unsigned char unknown_06[0x12];
    float x;
    float y;
} Source0881276C;

typedef struct Link0881276C {
    int kind;
    float x;
    float y;
    Source0881276C *source;
    unsigned char field_10[0x10];
} Link0881276C;

void func_089A34C0(void *p, int arg);

void func_0881276C(Link0881276C *self, int arg, Source0881276C *source) {
    int kind = source->kind;
    self->source = source;
    self->kind = kind;
    self->x = source->x;
    self->y = source->y;
    func_089A34C0(self->field_10, arg);
}
