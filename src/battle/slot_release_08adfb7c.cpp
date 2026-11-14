typedef struct SlotEntry18 {
    char data[0x18];
} SlotEntry18;

typedef struct SlotSet {
    SlotEntry18 slots[3];
    void *handles[4];
} SlotSet;

extern "C" {

extern void func_08ADF458(SlotEntry18 *slot);
extern void func_088DA060(void *handle);

void func_08ADFB7C(SlotSet *self)
{
    for (int i = 0; i < 3; i++) {
        SlotEntry18 *slot = &self->slots[i];
        func_08ADF458(slot);
    }
    for (int i = 0; i < 4; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
}

}
