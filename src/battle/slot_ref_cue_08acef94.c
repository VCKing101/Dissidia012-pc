typedef struct SlotRef {
    unsigned char active;
    int index;
    float value;
} SlotRef;

extern void *D_08A62280;
extern void func_08A620B0(void *sound, int id);

int func_08ACEF94(SlotRef *self)
{
    func_08A620B0(D_08A62280, 9);
    self->index = -1;
    self->value = 0.0f;
    return 0;
}
