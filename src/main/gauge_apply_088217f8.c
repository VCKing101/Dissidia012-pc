typedef struct Info088217F8 {
    unsigned char unknown_00[0xA];
    short flags;
} Info088217F8;

typedef struct Body088217F8 {
    unsigned char unknown_000[0x564];
    Info088217F8 *info;
} Body088217F8;

typedef struct Gauge088217F8 {
    unsigned char unknown_00[4];
    Body088217F8 *body;
    unsigned char unknown_08[8];
    signed char field_10;
    signed char percent;
} Gauge088217F8;

void func_08821864(Gauge088217F8 *self, void *part);
void func_08834DF4(Body088217F8 *body, float rate);

void func_088217F8(Gauge088217F8 *self) {
    if (!(self->body->info->flags & 1)) {
        return;
    }
    func_08821864(self, &self->field_10);
    func_08834DF4(self->body, (float)self->percent / 100.0f);
}
