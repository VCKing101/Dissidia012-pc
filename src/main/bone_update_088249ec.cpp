typedef struct Quat088249EC {
    float x;
    float y;
    float z;
    float w;
} Quat088249EC;

typedef struct Bone088249EC {
    unsigned char unknown_000[0x30];
    unsigned char field_030[0x50];
    unsigned char field_080[0x10];
    unsigned char field_090[0xA0];
} Bone088249EC;

typedef struct Skeleton088249EC {
    unsigned char unknown_00[0x20];
    unsigned int unknown_bits : 5;
    signed int bone_count : 8;
    unsigned char unknown_24[4];
    Bone088249EC *bones;
} Skeleton088249EC;

extern "C" void func_089A38FC(Quat088249EC *out, void *a, void *b);
extern "C" void func_089A3868(void *a, void *b);
extern "C" void func_089A38C4(void *out, void *in, Quat088249EC *q);

extern "C" void func_088249EC(Skeleton088249EC *self) {
    Quat088249EC q;
    for (int i = 0; i < self->bone_count; i++) {
        func_089A38FC(&q, self->bones[i].field_030, self->bones[i].field_080);
        func_089A3868(self->bones[i].field_080, self->bones[i].field_030);
        func_089A38C4(self->bones[i].field_090, self->bones[i].field_090, &q);
    }
}
