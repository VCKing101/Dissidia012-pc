typedef struct Child0881D6AC {
    unsigned char unknown_000[0x47C];
    int field_47C;
} Child0881D6AC;

typedef struct Owner0881D6AC {
    unsigned char unknown_00[0x14];
    Child0881D6AC *child;
} Owner0881D6AC;

int func_0881D6AC(void *self, Owner0881D6AC *owner) {
    if (owner == 0) {
        return 0;
    }
    if (owner->child == 0) {
        return 0;
    }
    if (owner->child->field_47C == 0) {
        return 0;
    }
    return 1;
}
