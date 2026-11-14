/* Stops a player: its stream, the paused-state handling, its sound and voice. */
typedef struct Player088CD4C8 {
    int field_00;
    int voice;
    int sound;
    char pad_0C[0x14];
    unsigned char paused;
} Player088CD4C8;

void func_088CD538(Player088CD4C8 *p);
void func_088CDC60(Player088CD4C8 *p);
void func_088DA060(int sound);
void func_088DAB44(int voice);
void func_088CD49C(Player088CD4C8 *p);
void func_088CD4C8(Player088CD4C8 *p)
{
    func_088CD538(p);
    if (p->paused == 0) {
        func_088CDC60(p);
    }
    if (p->sound != 0) {
        func_088DA060(p->sound);
        p->sound = 0;
    }
    if (p->voice != 0) {
        func_088DAB44(p->voice);
        p->voice = 0;
    }
    func_088CD49C(p);
}
