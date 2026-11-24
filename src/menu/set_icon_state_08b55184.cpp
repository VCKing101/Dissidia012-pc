// Picks an icon frame pair by mode (0, 1 or 2), applies it to the owner's sprite, then plays a sound.
struct Sprite { char pad[0xE]; short frame; };
struct Owner { Sprite *sprite; };
extern "C" {
extern void func_088D9F5C(Sprite *, int, int);
extern void func_088A24D4(int, int, int, int);
void func_08B55184(Owner *self, int mode) {
    switch (mode) {
    case 0:
        func_088D9F5C(self->sprite, 0, 1);
        self->sprite->frame = 1;
        break;
    case 1:
        func_088D9F5C(self->sprite, 4, 1);
        self->sprite->frame = 5;
        break;
    case 2:
        func_088D9F5C(self->sprite, 2, 1);
        self->sprite->frame = 3;
        break;
    }
    func_088A24D4(0, 0x61A85, 1, 0);
}
}
