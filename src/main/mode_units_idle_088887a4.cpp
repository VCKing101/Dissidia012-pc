/* Once every present unit reports ready, restarts each unit's motion 1 and
   moves the mode to state 1. */
extern "C" {
struct ModeUnit {
    char pad_000[0x130];
    char motion[4];
};

struct ModeCtl {
    char pad_00[0x38];
    ModeUnit *units[4];
    int state;
};

int func_08B2F9E0(ModeUnit *unit);
void func_0882FA10(void *motion, int id, float x, float y, int arg);

void func_088887A4(ModeCtl *self)
{
    for (int i = 0; i < 2; i++) {
        if (self->units[i] != 0) {
            if (func_08B2F9E0(self->units[i]) == 0) {
                return;
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        if (self->units[i] != 0) {
            func_0882FA10(self->units[i]->motion, 1, 0.0f, 0.0f, 0);
        }
    }
    self->state = 1;
}

}
