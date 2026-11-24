/* Stops the looping effects of one side (or both when side < 0); the first
   effect is kept when `keep_first` is set. */
extern "C" {
struct ModeCtl {
    char pad_000[0x7D8];
    int first[2];
    int first_id[2];
    char pad_7E8[0x7F0 - 0x7E8];
    int second[2];
    int second_id[2];
};

extern ModeCtl *D_089D6A10;
extern int D_089D4960;

int func_088444D4(int pool, int effect);
void func_08844DE0(int effect);

void func_0888A2B4(int side, unsigned char keep_first)
{
    ModeCtl *mode = D_089D6A10;

    for (int slot = 0; slot < 2; slot++) {
        if (side >= 0 && side != slot) {
            continue;
        }
        if (!keep_first) {
            if (mode->first[slot] != -1) {
                if (func_088444D4(D_089D4960, mode->first[slot])) {
                    func_08844DE0(mode->first[slot]);
                }
            }
            mode->first[slot] = -1;
            mode->first_id[slot] = -1;
        }
        if (mode->second[slot] != -1) {
            if (func_088444D4(D_089D4960, mode->second[slot])) {
                func_08844DE0(mode->second[slot]);
            }
        }
        mode->second[slot] = -1;
        mode->second_id[slot] = -1;
    }
}

}
