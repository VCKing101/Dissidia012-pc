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
void func_08808468(int a, int id, int b, float *values, unsigned char c)
{
    for (int i = 0; i < 32; i++) {
        Slot *slot = &D_09AF84F0[i];
        if (slot->id < 0) {
            slot->a = a;
            slot->b = b;
            for (int j = 0; j < 3; j++) {
                slot->values[j] = values[j];
            }
            slot->id = id;
            slot->c = c;
            return;
        }
    }
}
}
