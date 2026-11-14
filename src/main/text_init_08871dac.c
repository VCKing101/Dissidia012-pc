/* Initialises a text node with the default colour pair and margins. */
typedef struct Text08871DAC {
    char pad_00[0x44];
    int field_44;
    unsigned char margin_x;
    unsigned char margin_y;
} Text08871DAC;

extern unsigned long long D_089C9A68;
extern float D_089C972C;

void func_08871B40(Text08871DAC *t, int arg, unsigned long long colors);

void func_08871DAC(Text08871DAC *t, int arg)
{
    func_08871B40(t, arg, D_089C9A68);
    t->field_44 = 0;
    t->margin_x = (int)D_089C972C;
    t->margin_y = (int)D_089C972C;
}
