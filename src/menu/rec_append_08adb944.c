typedef struct AppendRec {
    short field_00;
    short field_02;
    char field_04;
    char pad_05[3];
} AppendRec;

typedef struct AppendCtx {
    char pad_0000[0x1CB0];
    int field_1CB0;
    AppendRec field_1CB4[1];
} AppendCtx;

typedef struct AppendOwner {
    char pad_000[0x46AC];
    AppendCtx *field_46AC;
} AppendOwner;

void func_08ADB944(AppendOwner *self, short *source, int unused, unsigned char kind, unsigned short extra)
{
    *(short *)((char *)self->field_46AC + self->field_46AC->field_1CB0 * 8 + 0x1CB4) = *source;
    *(short *)((char *)self->field_46AC + self->field_46AC->field_1CB0 * 8 + 0x1CB6) = extra;
    *(char *)((char *)self->field_46AC + self->field_46AC->field_1CB0 * 8 + 0x1CB8) = kind;
    self->field_46AC->field_1CB0 = self->field_46AC->field_1CB0 + 1;
}
