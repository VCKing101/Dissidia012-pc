struct Voice { char data[0x84C]; };
struct VoiceSet {
    char pad_00[0x4];
    int bank;
    char pad_08[0x10];
    int source;
    Voice voices[2];
};
extern "C" {
extern int D_089EA528[2];
extern void func_089E8EF0(int bank);
extern unsigned int func_089E95E0(int index);
extern void func_088DA0A8(int source, int id, char *params, int *extra);
extern void func_089EA86C(Voice *voice, char *params, int *extra);

/* Loads the parameters for each voice whose kind is known. */
int func_089EA450(VoiceSet *self)
{
    char params[0x40];
    int extra;
    func_089E8EF0(self->bank);
    for (int i = 0; i < 2; i++) {
        unsigned int kind = func_089E95E0(i);
        if (kind >= 2) {
            continue;
        }
        func_088DA0A8(self->source, D_089EA528[kind], params, &extra);
        func_089EA86C(&self->voices[i], params, &extra);
    }
    return 0;
}
}
