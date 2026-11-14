/* Copies the current rotation into the unit and returns its w component. */
struct Quat08835B3C {
    float x;
    float y;
    float z;
    float w;
};

struct Unit08835B3C {
    char pad_00[0x90];
    Quat08835B3C rotation;
};

extern "C" {

Quat08835B3C func_08867F08(void);
void func_089A3868(Quat08835B3C *dst, Quat08835B3C *src);

float func_08835B3C(Unit08835B3C *u)
{
    Quat08835B3C q;
    q = func_08867F08();
    func_089A3868(&u->rotation, &q);
    return u->rotation.w;
}

}
