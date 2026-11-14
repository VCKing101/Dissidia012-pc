typedef struct AnimHandle {
    char pad_00[0xC];
    short current;
    short speed;
} AnimHandle;

typedef struct AnimSync {
    unsigned char active;
    char pad_01[0x3];
    AnimHandle *anim;
    char pad_08[0x20];
    int speed;
    char pad_2C[0x4];
    int motion;
} AnimSync;

extern void func_088D9F5C(AnimHandle *anim, int motion, int restart);

int func_08A00C68(AnimSync *self)
{
    AnimHandle *anim;
    if (!self->active) {
        return -1;
    }
    anim = self->anim;
    if (anim != 0 && anim->current != self->motion) {
        func_088D9F5C(anim, self->motion, 1);
        anim = self->anim;
        anim->speed = self->speed;
    }
    return 0;
}
