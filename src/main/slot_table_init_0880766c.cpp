extern "C" {
struct Slot {
    int a;
    int b;
    float values[3];
    short id;
    unsigned char c;
    unsigned char d;
};
extern Slot D_09AF84F0[32];
void func_0880766C(void)
{
    for (int i = 0; i < 32; i++) {
        Slot *slot = &D_09AF84F0[i];
        slot->a = 0;
        slot->b = 0;
        for (int j = 0; j < 3; j++) {
            slot->values[j] = 0.0f;
        }
        slot->id = -1;
        slot->c = 0;
        slot->d = 0;
    }
}
}
