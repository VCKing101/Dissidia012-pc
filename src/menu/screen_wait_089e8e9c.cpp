/* Waits for the sound system and the screen's parts, then switches the screen (state 3). */
class SoundSystem {
    char data[0x34];
public:
    virtual void slot_08();
    virtual int slot_10(unsigned long long mask);
};

struct ScreenParts;

struct WaitScreen {
    char pad_000[0x140];
    ScreenParts *field_140;
    char pad_144[0x28];
    int field_16C;
};

extern "C" {
SoundSystem *func_088729A0(int index);
int func_089EA9F4(ScreenParts *parts);
void func_089EA994(ScreenParts *parts);
int func_088D4C5C(void);
void func_088A24D4(int a, int id, int b, int c);
void func_089E9348(WaitScreen *self, int screen, void (*next)(void));
void func_089E9460(void);
extern unsigned long long D_089E9CF0;

void func_089E8E9C(WaitScreen *self)
{
    if (func_088729A0(0)->slot_10(D_089E9CF0) != 0) {
        if (func_089EA9F4(self->field_140) == 1) {
            int screen = func_088D4C5C();
            func_088A24D4(0, 0x2711, 1, 0);
            func_089E9348(self, screen, func_089E9460);
            self->field_16C = 3;
        } else {
            func_089EA994(self->field_140);
        }
    }
}
}
