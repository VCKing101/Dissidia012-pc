/* Reads the matrix and colour for the current mode and applies both to the model. */
struct State08B072D4 {
    char pad_00000[0x484];
    void *field_484;
    char pad_00488[0x48C - 0x488];
    void *field_48C;
    char pad_00490[0x1F8C0 - 0x490];
    int field_1F8C0;
};

struct Owner08B072D4 {
    char pad_000[0x138];
    struct State08B072D4 *field_138;
};

extern unsigned short D_08B1C65E[];
extern void func_088DA0A8(void *table, int id, float *matrix, unsigned char *color);
extern void func_088D9B84(void *model, float *matrix);
extern void func_088D9AF4(void *model, unsigned char *color);

void func_08B072D4(struct Owner08B072D4 *self)
{
    float matrix[16];
    unsigned char color[4];
    func_088DA0A8(self->field_138->field_48C, D_08B1C65E[self->field_138->field_1F8C0], matrix, color);
    func_088D9B84(self->field_138->field_484, matrix);
    func_088D9AF4(self->field_138->field_484, color);
}
