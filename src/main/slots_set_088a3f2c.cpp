/* Sets a byte option on every used stream slot. */
struct Slot088A3F2C {
    int id;
    char inner[4];
};

extern "C" {
extern Slot088A3F2C *D_09B4D418[2];
void func_0889F898(void *inner, unsigned char value);
}

struct Slots088A3F2C {
    static void set(Slot088A3F2C *s, unsigned char value) { func_0889F898(s->inner, value); }
};

extern "C" {

void func_088A3F2C(unsigned char value)
{
    for (unsigned int i = 0; i < 2; i++) {
        if (D_09B4D418[i] != 0) {
            Slots088A3F2C::set(D_09B4D418[i], value);
        }
    }
}

}
