/* Reads the three mark colours and applies them to their parts, at half
   alpha for marks that are off (-1 keeps the part's own alpha). */
struct State08ADC5AC {
    char pad_0000[0x48C];
    void *field_48C;
    char pad_0490[0x880 - 0x490];
    void *field_880[(0x46A4 - 0x880) / 4];
    unsigned char field_46A4[3];
};

struct Screen08ADC5AC {
    char pad_0000[0x46AC];
    struct State08ADC5AC *field_46AC;
};

extern unsigned short D_08ADFB9E[3];
extern unsigned short D_08ADFBA4[3];
extern void func_088DA0A8(void *table, int id, float *matrix, unsigned char *color);
extern void func_0887B8A8(void *part, int alpha);

void func_08ADC5AC(struct Screen08ADC5AC *self)
{
    float matrix[16];
    unsigned char color[4];
    int i;
    for (i = 0; i < 3; i++) {
        int alpha;
        func_088DA0A8(self->field_46AC->field_48C, D_08ADFB9E[i], matrix, color);
        alpha = -1;
        if (self->field_46AC->field_46A4[i] == 0) {
            alpha = color[3] >>= 1;
        }
        func_0887B8A8(self->field_46AC->field_880[D_08ADFBA4[i]], alpha);
    }
}
