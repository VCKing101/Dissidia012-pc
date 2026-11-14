/* While flag 0x20 is set, steps the animation; when it finishes, clears the
   flag and runs the follow-up. */
struct Object08B3561C {
    char pad_00[0x44];
    unsigned int field_44;
    char pad_48[0xA8 - 0x48];
    void *field_A8;
};

extern "C" {
int func_088EB17C(void *anim);
void func_088EB65C(void *anim);
void func_08B3CAD8(void);
}

struct Flags08B3561C {
    static bool has(unsigned int flags, unsigned int mask) { return (flags & mask) != 0; }
};

extern "C" {
void func_08B3561C(Object08B3561C *self)
{
    if (Flags08B3561C::has(self->field_44, 0x20)) {
        if (func_088EB17C(self->field_A8) != 0) {
            self->field_44 &= ~0x20;
            func_08B3CAD8();
        }
        func_088EB65C(self->field_A8);
    }
}
}
