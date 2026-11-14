typedef struct TickPage {
    char pad_0000[0x400];
    char field_400[0x480];
    char field_880[0x110C - 0x880];
    int field_110C;
} TickPage;

typedef struct TickOwner {
    char pad_0000[0x1320];
    TickPage *field_1320;
} TickOwner;

extern char D_08A11AAE[];
extern void func_08A0ECF0(TickOwner *self);
extern void func_088D44EC(char *dst, char *src, char *name);

void func_08A0CBC0(TickOwner *self)
{
    if (self->field_1320->field_110C != 0) {
        self->field_1320->field_110C--;
        if (self->field_1320->field_110C <= 0) {
            self->field_1320->field_110C = 0;
            func_08A0ECF0(self);
        }
    }
    func_088D44EC(self->field_1320->field_400, self->field_1320->field_880, D_08A11AAE);
}
