/* Checks the reference; with the unlock option the rank of its group is included. */
typedef struct CheckRefTarget {
    char pad_00[0xD];
    signed char field_0D;
} CheckRefTarget;

typedef struct CheckRef {
    CheckRefTarget *field_00;
} CheckRef;

typedef struct CheckShared {
    char pad_000[0xE6];
    unsigned char field_E6;
} CheckShared;

extern CheckShared D_09B6CDA0;
extern int func_088C1998(CheckRef *ref);
extern int func_08AF8DA4(void *self, int bit);
extern int func_088C1A5C(CheckRef *ref, int mask);

int func_08AF8E14(void *self, CheckRef *ref)
{
    CheckRefTarget *target = ref->field_00;
    CheckShared *shared;
    if (target == 0) {
        return 1;
    }
    shared = &D_09B6CDA0;
    if (shared->field_E6 == 0) {
        return func_088C1998(ref);
    }
    {
        int mask = func_088C1998(ref);
        return func_088C1A5C(ref, mask | func_08AF8DA4(self, target->field_0D));
    }
}
