/* Resets the three per-slot tables of a five-slot page. */
typedef struct SlotHead {
    int field_00;
    int pad_04[3];
} SlotHead;

typedef struct SlotBody {
    int field_00;
    int pad_04[9];
} SlotBody;

typedef struct SlotTail {
    int field_00;
    int pad_04[0x57];
} SlotTail;

typedef struct SlotPage {
    unsigned char field_00;
    char pad_01[3];
    int field_04;
    int field_08;
    int field_0C;
    SlotHead field_10[5];
    SlotBody field_60[5];
    SlotTail field_128[5];
} SlotPage;

void func_08A0B990(SlotPage *self)
{
    int i;
    for (i = 0; i < 5; i++) {
        self->field_10[i].field_00 = 0;
        self->field_60[i].field_00 = 0;
        self->field_128[i].field_00 = -1;
    }
    self->field_00 = 0;
    self->field_04 = -1;
    self->field_0C = 0;
}
