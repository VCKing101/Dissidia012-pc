extern int func_08A22E2C(void *, int, int, int);
extern float func_08891348(int);
extern void func_08A16CC4(void *, float);
extern void func_08A01C74(void *, int);
extern void func_08A16C58(void *);

typedef struct Query0514 {
    int field_00;
    int field_04;
    float field_08;
    char pad_00C[0x20];
} Query0514;

typedef struct Track0514 {
    char pad_000[0x8];
    int field_08;
    char pad_00C[0x14];
    char field_20[0x28];
    char field_48[0xB0];
    int field_F8;
    int field_FC;
    int field_100;
} Track0514;

int func_08A00514(Track0514 *self)
{
    Query0514 query;
    if (!func_08A22E2C(&query, self->field_F8, self->field_FC, 6)) {
        float scale = func_08891348(self->field_100);
        if (scale != 0.0f) {
            func_08A16CC4(self->field_48, query.field_08 / scale);
        }
    } else {
        func_08A01C74(self->field_20, 3);
        func_08A16C58(self->field_48);
        self->field_08 = 4;
    }
    return 0;
}
