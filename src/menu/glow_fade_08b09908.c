/* Reads colour 4 and applies it to the glow part, at half alpha when the
   glow is off (-1 keeps the part's alpha). */
struct State08B09908 {
    char pad_00000[0x494];
    void *field_494;
    char pad_00498[0x9F0 - 0x498];
    void *field_9F0;
    char pad_009F4[0x2461C - 0x9F4];
    unsigned char field_2461C;
};

struct Owner08B09908 {
    char pad_000[0x138];
    struct State08B09908 *field_138;
};

extern void func_088DA0A8(void *table, int id, float *matrix, unsigned char *color);
extern void func_0887B8A8(void *part, int alpha);

void func_08B09908(struct Owner08B09908 *self)
{
    float matrix[16];
    unsigned char color[4];
    int alpha;
    func_088DA0A8(self->field_138->field_494, 4, matrix, color);
    alpha = -1;
    if (self->field_138->field_2461C == 0) {
        alpha = color[3] >>= 1;
    }
    func_0887B8A8(self->field_138->field_9F0, alpha);
}
