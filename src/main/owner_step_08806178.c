typedef struct Agent4Owner06178 {
    unsigned char unknown_0000[0x394];
    int field_0394;
    int field_0398;
    unsigned char unknown_039C[0x394];
    unsigned char field_0730[0x3C0];
    unsigned char field_0AF0;
} Agent4Owner06178;

void func_08804C94(Agent4Owner06178 *self);
void func_088050E0(Agent4Owner06178 *self);
void func_08830D6C(void *part, int value);
void func_0880EA20(int handle, int arg, Agent4Owner06178 *self);
void func_08805A38(Agent4Owner06178 *self);

void func_08806178(Agent4Owner06178 *self, int arg) {
    func_08804C94(self);
    if (self->field_0AF0 != 0) {
        func_088050E0(self);
        func_08830D6C(self->field_0730, self->field_0394);
    }
    func_0880EA20(self->field_0398, arg, self);
    func_08805A38(self);
}
