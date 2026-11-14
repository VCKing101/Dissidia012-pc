/* Tints the item model with the colour of the current mode, when a selection
   exists and its entry is in use. */
struct Item08B2B46C {
    char pad_00[0xE4];
    void *field_E4;
    char pad_E8[0xF0 - 0xE8];
    void *field_F0;
};

struct Entry08B2B46C {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
};

struct State08B2B46C {
    char pad_000[0x864];
    int field_864;
    char pad_868[0x870 - 0x868];
    int field_870;
};

struct Owner08B2B46C {
    char pad_000[0xA18];
    struct State08B2B46C *field_A18;
};

extern struct Entry08B2B46C D_08B2D250[];
extern unsigned short D_08B2D260[];
extern void func_088DA0A8(void *table, int id, float *matrix, unsigned char *color);
extern void func_088D9B84(void *model, float *matrix);
extern void func_088D9AF4(void *model, unsigned char *color);

void func_08B2B46C(struct Owner08B2B46C *self, struct Item08B2B46C *item)
{
    float matrix[16];
    unsigned char color[4];
    if (self->field_A18->field_864 == -1) {
        return;
    }
    if (D_08B2D250[self->field_A18->field_864].field_4 == -1) {
        return;
    }
    func_088DA0A8(item->field_F0, D_08B2D260[self->field_A18->field_870], matrix, color);
    func_088D9B84(item->field_E4, matrix);
    func_088D9AF4(item->field_E4, color);
}
