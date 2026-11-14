typedef struct Child0881D55C {
    unsigned char unknown_000[0x5F8];
    int field_5F8;
} Child0881D55C;

typedef struct Owner0881D55C {
    unsigned char unknown_00[0x14];
    Child0881D55C *child;
} Owner0881D55C;

int func_0881D55C(void *self, Owner0881D55C *owner) {
    if (owner == 0) {
        return 0;
    }
    if (owner->child == 0) {
        return 0;
    }
    if (owner->child->field_5F8 == 0) {
        return 0;
    }
    return 1;
}
