extern "C" {
typedef struct CardBody { char pad_000[0xA1C]; int field_A1C; int field_A20; char names[64][64]; char texts[64][64]; } CardBody;
typedef struct CardUnit { char pad_00[0x20]; CardBody *body; } CardUnit;
extern void func_08935828(void *dst, int value, int size);
void func_089265B8(CardUnit *unit)
{
    unit->body->field_A1C = 0;
    unit->body->field_A20 = 0;
    for (int i = 0; i < 64; i++) {
        func_08935828(unit->body->names[i], 0, 64);
        func_08935828(unit->body->texts[i], 0, 64);
    }
}
}
