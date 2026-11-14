/* Reads the matrix and colour for the current mode and applies both to the model. */
struct State08B071A0 {
    char pad_00000[0x68C];
    void *field_68C;
    void *field_690;
    char pad_00694[0x1F98C - 0x694];
    int field_1F98C;
};

struct Owner08B071A0 {
    char pad_000[0x138];
    struct State08B071A0 *field_138;
};

extern unsigned short D_08B1CA3C[];
extern void func_088DA0A8(void *table, int id, float *matrix, unsigned char *color);
extern void func_088D9B84(void *model, float *matrix);
extern void func_088D9AF4(void *model, unsigned char *color);

void func_08B071A0(struct Owner08B071A0 *self)
{
    float matrix[16];
    unsigned char color[4];
    func_088DA0A8(self->field_138->field_690, D_08B1CA3C[self->field_138->field_1F98C], matrix, color);
    func_088D9B84(self->field_138->field_68C, matrix);
    func_088D9AF4(self->field_138->field_68C, color);
}
