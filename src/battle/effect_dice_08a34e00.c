/* Rolls the hit effect's three dice faces (each 1 to 7 steps on from the
 * last, wrapping at 8; the seed advances once more after the rolls) and
 * shows them on the dice sprites. */
typedef struct DiceFace {
    int face;
    int pad_04;
} DiceFace;

typedef struct DiceParts {
    unsigned char pad_00[0x10];
    DiceFace faces[3];
    void *sprites[3];
    unsigned char pad_34[0xC];
    void *layout;
} DiceParts;

typedef struct DiceEffect {
    unsigned char pad_00[0x14];
    DiceParts *parts;
} DiceEffect;

typedef struct DiceMatrix {
    float m[16];
} DiceMatrix;

extern int D_09AF8B00;

extern void func_088DA0A8(void *layout, int slot, DiceMatrix *matrix, int *flags);
extern void func_088D9F5C(void *sprite, int frame, int flags);
extern void func_088D9B84(void *sprite, DiceMatrix *matrix);
extern void func_088D9CFC(void *sprite);

void func_08A34E00(DiceEffect *self)
{
    int face = 0;
    int i;
    DiceMatrix matrix;
    int flags;
    for (i = 0; i < 3; i++) {
        face = (face + ((D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF) % 7 + 1) % 8;
        self->parts->faces[i].face = face;
    }
    D_09AF8B00 = D_09AF8B00 * 0x343FD + 0x269EC3;
    for (i = 0; i < 3; i++) {
        func_088DA0A8(self->parts->layout, i, &matrix, &flags);
        func_088D9F5C(self->parts->sprites[i], self->parts->faces[i].face * 2, 1);
        func_088D9B84(self->parts->sprites[i], &matrix);
        func_088D9CFC(self->parts->sprites[i]);
    }
}
