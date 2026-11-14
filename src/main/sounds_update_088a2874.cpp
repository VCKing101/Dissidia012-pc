struct Sound { char data_00[0x58]; Sound *next; };
extern "C" {
extern Sound *D_09B4CF98;
}
struct Sounds {
    static Sound *after(Sound *s) { return s == 0 ? D_09B4CF98 : s->next; }
};
extern "C" {
void func_088A1F2C(Sound *s, float volume, float pan);

void func_088A2874(float volume, float pan)
{
    Sound *s = 0;
    while ((s = Sounds::after(s)) != 0) {
        func_088A1F2C(s, volume, pan);
    }
}
}
