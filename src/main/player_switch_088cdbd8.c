/* Switches a ready, playing player to another value; -1 when not ready. */
typedef struct Player088CDBD8 {
    int field_00;
    int voice;
    int sound;
    char pad_0C[0x10];
    int value;
} Player088CDBD8;

int func_088CD9D4(Player088CDBD8 *p);
void func_088CD538(Player088CDBD8 *p);
void func_088CD754(Player088CDBD8 *p);
int func_088CDBD8(Player088CDBD8 *p, int value)
{
    if (func_088CD9D4(p) == 0) {
        return -1;
    }
    if (p->sound == 0) {
        return 0;
    }
    if (p->value == value) {
        return 0;
    }
    func_088CD538(p);
    p->value = value;
    func_088CD754(p);
    return 0;
}
