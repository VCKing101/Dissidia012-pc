typedef struct IdValue {
    int id;
    float value;
} IdValue;

typedef struct SlotState {
    unsigned char active;
    int field_04;
    int index;
    float value;
    int field_10;
    IdValue slots[1];
    int field_1C;
} SlotState;

extern "C" {

void func_08AC8F84(SlotState *self)
{
    self->active = 0;
    self->field_04 = 0;
    self->index = -1;
    self->value = 0.0f;
    self->field_10 = 0;
    for (int i = 0; i < 1; i++) {
        IdValue *slot = &self->slots[i];
        slot->id = -1;
        slot->value = 0.0f;
    }
    self->field_1C = 0;
}

}
