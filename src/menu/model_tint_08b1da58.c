/* Reads the matrix and colour for the page offset and applies both to the model. */
struct State08B1DA58 {
    char pad_0000[0x480];
    void *field_480;
    char pad_0484[0x4A8 - 0x484];
    void *field_4A8;
    char pad_04AC[0x8124 - 0x4AC];
    int field_8124;
    char pad_8128[0x8130 - 0x8128];
    int field_8130;
};

struct Owner08B1DA58 {
    char pad_000[0x138];
    struct State08B1DA58 *field_138;
};

extern unsigned short D_08B22DD0[];
extern void func_088DA0A8(void *table, int id, float *matrix, unsigned char *color);
extern void func_088D9B84(void *model, float *matrix);
extern void func_088D9AF4(void *model, unsigned char *color);

void func_08B1DA58(struct Owner08B1DA58 *self)
{
    float matrix[16];
    unsigned char color[4];
    func_088DA0A8(self->field_138->field_4A8, D_08B22DD0[self->field_138->field_8124 - self->field_138->field_8130], matrix, color);
    func_088D9B84(self->field_138->field_480, matrix);
    func_088D9AF4(self->field_138->field_480, color);
}
