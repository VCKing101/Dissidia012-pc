/* Copies the selected row model's matrix and colour onto both cursor models, then sets the help text. */
struct FrameState {
    char pad_000[0x88];
    char field_88[0x400];
    char field_488[4];
    char pad_48C[0x90];
    void *field_51C[9 * 10]; /* rows of ten words */
    char pad_684[0x4];
    void *field_688;
    void *field_68C;
    char pad_690[0x2094];
    unsigned int field_2724;
    static void *word(FrameState *state, int index) { return state->field_51C[index]; }
};

struct FrameOwner {
    char pad_000[0x14C];
    FrameState *field_14C;
};

extern "C" {
int func_089F2FF0(FrameOwner *self);
void func_088DA0A8(void *model, int part, float *matrix, int *color);
void func_088D9B84(void *model, float *matrix);
void func_088D9AF4(void *model, int *color);
void func_088D3F00(char *text, char *work, unsigned short *message);
extern unsigned short D_08A09A4A[];

void func_089F63C8(FrameOwner *self)
{
    float matrix[16];
    int color;
    int row;

    if ((self->field_14C->field_2724 & 1) != 0) {
        row = func_089F2FF0(self);
        func_088DA0A8(FrameState::word(self->field_14C, row * 10), 1, matrix, &color);
        func_088D9B84(self->field_14C->field_688, matrix);
        func_088D9AF4(self->field_14C->field_688, &color);
        func_088D9B84(self->field_14C->field_68C, matrix);
        func_088D9AF4(self->field_14C->field_68C, &color);
        func_088D3F00(self->field_14C->field_88, self->field_14C->field_488, D_08A09A4A);
    }
}
}
