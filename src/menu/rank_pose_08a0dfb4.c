/* Sets the rank texts, then copies the rank model's pose (matrix and colour) onto the badge model. */
typedef struct RankState {
    char field_000[0x400];
    char field_400[4];
    char pad_404[0x1C4];
    void *field_5C8;
    char pad_5CC[0xC];
    void *field_5D8;
    char pad_5DC[0x2A4];
    char field_880[4];
    char pad_884[0x78C];
    unsigned int field_1010;
    char pad_1014[0xB4];
    int field_10C8;
} RankState;

typedef struct RankOwner {
    char pad_0000[0x1320];
    RankState *field_1320;
} RankOwner;

extern void func_088D3F00(char *text, char *work, unsigned short *message);
extern void func_088D44EC(char *work, char *out, unsigned short *format);
extern void func_088DA0A8(void *model, int part, float *matrix, int *color);
extern void func_088D9B84(void *model, float *matrix);
extern void func_088D9AF4(void *model, int *color);
extern unsigned short D_08A11B7E[];
extern unsigned short D_08A11BBE[];
extern unsigned short D_08A11D04[];

void func_08A0DFB4(RankOwner *self)
{
    float matrix[16];
    int color;
    int rank;

    if ((self->field_1320->field_1010 & 2) == 0) {
        return;
    }
    rank = self->field_1320->field_10C8;
    func_088D3F00(self->field_1320->field_000, self->field_1320->field_400, D_08A11B7E);
    func_088D44EC(self->field_1320->field_400, self->field_1320->field_880, D_08A11BBE);
    func_088DA0A8(self->field_1320->field_5C8, D_08A11D04[rank], matrix, &color);
    func_088D9B84(self->field_1320->field_5D8, matrix);
    func_088D9AF4(self->field_1320->field_5D8, &color);
}
