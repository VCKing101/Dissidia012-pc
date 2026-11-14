/* Reads the matrix and colour for the current mode and applies both to the model. */
struct State08B23300 {
    char pad_000[0x484];
    void *field_484;
    void *field_488;
    char pad_48C[0x9B0 - 0x48C];
    int field_9B0;
};

struct Owner08B23300 {
    char pad_000[0x138];
    struct State08B23300 *field_138;
};

extern unsigned short D_08B243A4[];
extern void func_088DA0A8(void *table, int id, float *matrix, unsigned char *color);
extern void func_088D9B84(void *model, float *matrix);
extern void func_088D9AF4(void *model, unsigned char *color);

void func_08B23300(struct Owner08B23300 *self)
{
    float matrix[16];
    unsigned char color[4];
    func_088DA0A8(self->field_138->field_488, D_08B243A4[self->field_138->field_9B0], matrix, color);
    func_088D9B84(self->field_138->field_484, matrix);
    func_088D9AF4(self->field_138->field_484, color);
}
