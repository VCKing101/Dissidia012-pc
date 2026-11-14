/* Reads the matrix and colour for the current mode and applies both to the model. */
struct State08B09238 {
    char pad_00000[0x6E8];
    void *field_6E8;
    char pad_006EC[0x6F4 - 0x6EC];
    void *field_6F4;
    char pad_006F8[0x1F960 - 0x6F8];
    int field_1F960;
};

struct Owner08B09238 {
    char pad_000[0x138];
    struct State08B09238 *field_138;
};

extern unsigned short D_08B1CA86[];
extern void func_088DA0A8(void *table, int id, float *matrix, unsigned char *color);
extern void func_088D9B84(void *model, float *matrix);
extern void func_088D9AF4(void *model, unsigned char *color);

void func_08B09238(struct Owner08B09238 *self)
{
    float matrix[16];
    unsigned char color[4];
    func_088DA0A8(self->field_138->field_6E8, D_08B1CA86[self->field_138->field_1F960], matrix, color);
    func_088D9B84(self->field_138->field_6F4, matrix);
    func_088D9AF4(self->field_138->field_6F4, color);
}
