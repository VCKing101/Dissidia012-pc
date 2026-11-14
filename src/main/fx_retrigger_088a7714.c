/* Restarts an active effect whose owner allows it, unless it already plays id. */
typedef struct Owner088A7714 {
    char pad_00[0x7C];
    unsigned int mode;
} Owner088A7714;

typedef struct Fx088A7714 {
    char pad_00[0xC0];
    Owner088A7714 *owner;
    char pad_C4[0x18];
    unsigned int flags;
    char pad_E0[0x18];
    int id;
} Fx088A7714;

void func_088A7890(Fx088A7714 *fx);

void func_088A7714(Fx088A7714 *fx, int unused, int id)
{
    if (fx->flags & 1) {
        unsigned int mode = fx->owner->mode;
        if ((mode & 0x100) && !(mode & 0x80)) {
            if (fx->id != id) {
                func_088A7890(fx);
            }
        }
    }
}
