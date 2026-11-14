/* Sets the sub-state to 0x13, starts the part at +0x100 and, unless the
   global flag D_089D5F20 is set, the part at +0x1E0.
   Retail keeps `self` in the frame (0($sp)) across the first call and
   reloads it after reading the flag. */
template <class T> struct Kept {
    T value;
    Kept(T v) : value(v) {}
};

struct State089DE990 {
    int unknown_00;
    int unknown_04;
    int state;
};

struct Scene089DE990 {
    char unknown_000[0x1DC];
    State089DE990 *state;
};

extern "C" {

extern unsigned char D_089D5F20;
void func_08865158(void *part);
void func_0886243C(void *part);

void func_089DE990(Scene089DE990 *self)
{
    Kept<Scene089DE990 *> saved(self);
    self->state->state = 0x13;
    func_08865158((char *)self + 0x100);
    unsigned char skip = D_089D5F20;
    self = saved.value;
    if (skip == 0) {
        func_0886243C((char *)self + 0x1E0);
    }
}

}
