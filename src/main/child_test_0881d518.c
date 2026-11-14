typedef struct Child0881D518 {
    unsigned char unknown_000[0x5A0];
    signed char field_5A0;
} Child0881D518;

typedef struct Owner0881D518 {
    unsigned char unknown_00[0x14];
    Child0881D518 *child;
} Owner0881D518;

int func_0881D518(void *self, Owner0881D518 *owner) {
    if (owner == 0) {
        return 0;
    }
    if (owner->child == 0) {
        return 0;
    }
    if (owner->child->field_5A0 == 0) {
        return 0;
    }
    return 1;
}
