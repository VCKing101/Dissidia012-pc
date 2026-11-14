/* Releases the player's sound and voice, then finishes it. */
typedef struct Player088CE3D8 {
    int field_00;
    int voice;
    int sound;
} Player088CE3D8;

void func_088DA060(int sound);
void func_088DAB44(int voice);
void func_088CE3C4(Player088CE3D8 *p);

void func_088CE3D8(Player088CE3D8 *p)
{
    if (p->sound != 0) {
        func_088DA060(p->sound);
        p->sound = 0;
    }
    if (p->voice != 0) {
        func_088DAB44(p->voice);
        p->voice = 0;
    }
    func_088CE3C4(p);
}
