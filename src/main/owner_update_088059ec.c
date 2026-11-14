typedef struct Agent4Owner059EC {
    unsigned char unknown_0000[0xA90];
    unsigned char field_0A90[0x60];
    unsigned char field_0AF0;
    unsigned char unknown_0AF1[0xF];
    unsigned char field_0B00[0x10];
} Agent4Owner059EC;

void func_08806E28(void *part, Agent4Owner059EC *owner);
void func_08806DB8(void *part, Agent4Owner059EC *owner);

void func_088059EC(Agent4Owner059EC *self) {
    func_08806E28(self->field_0B00, self);
    if (self->field_0AF0 == 0) {
        return;
    }
    func_08806DB8(self->field_0A90, self);
}
