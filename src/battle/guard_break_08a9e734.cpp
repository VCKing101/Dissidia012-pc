struct GuardBody {
    char pad_000[0x180];
    short motion;
};

struct GuardSlot {
    char pad_00[0x4];
    GuardBody *body;
    char pad_08[0x28];
    int state;
};

struct GuardTask {
    char pad_000[0x4];
    int mode;
    char pad_008[0x150];
    int sound;
    char pad_15C[0x4];
    int id;
};

extern "C" {
extern int D_08ACA350;
extern GuardSlot *func_08AB3264(int id);
extern int func_08A65904(int sound);
extern int func_08AC92AC(int key);
}

struct GuardRules {
    static unsigned char breaking(GuardBody *body)
    {
        int result = 0;
        if (func_08AC92AC(D_08ACA350) == 1 || body->motion == 5000) {
            result = 1;
        }
        return result;
    }
};

extern "C" {

/* 1 when the guard should break: silent in mode 0x12, the break flag, or break motion 5000. */
unsigned char func_08A9E734(GuardTask *self)
{
    if (func_08AB3264(self->id)->state == 2) {
        return 0;
    }
    GuardBody *body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return 0;
    }
    int playing = func_08A65904(self->sound);
    if (self->mode == 0x12 && playing == 0) {
        return 1;
    }
    return GuardRules::breaking(body);
}

}
