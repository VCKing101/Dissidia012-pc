/* Stops the player's sound and voice and resets it. */
typedef struct Player088CC51C {
    int sound;
    int state;
    int voice;
    int id;
} Player088CC51C;

void func_088DA060(int sound);
void func_0887BA5C(int voice);
void func_088CC51C(Player088CC51C *p)
{
    if (p->sound != 0) {
        func_088DA060(p->sound);
        p->sound = 0;
    }
    if (p->voice != 0) {
        func_0887BA5C(p->voice);
        p->voice = 0;
    }
    p->state = 0;
    p->id = -1;
}
