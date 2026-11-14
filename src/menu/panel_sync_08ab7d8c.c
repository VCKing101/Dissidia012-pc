typedef struct SyncObj {
    char pad_000[0x400];
    char field_400[1];
    char pad_401[0x880 - 0x401];
    int field_880;
} SyncObj;

typedef struct SyncHolder {
    SyncObj *field_00;
} SyncHolder;

extern void func_088D3AE8(void *a, int b, int c, int d);
extern void func_088D3F00(void *a, void *b, short *table);
extern short D_08AB7F18;
extern unsigned short D_08AB7F34[];

void func_08AB7D8C(SyncHolder *self)
{
    func_088D3F00(self->field_00, self->field_00->field_400, &D_08AB7F18);
    func_088D3AE8(self->field_00->field_400, 0, D_08AB7F34[self->field_00->field_880], 1);
}
