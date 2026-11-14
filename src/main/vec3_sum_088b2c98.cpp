/* Clears the third vector and adds the first two into it. */
struct Vec088B2C98 {
    int x;
    int y;
    int z;
};

struct Triple088B2C98 {
    Vec088B2C98 pair[2];
    Vec088B2C98 third;
};

extern "C" {

void func_088B2B14(Vec088B2C98 *v);
void func_088B2B24(Vec088B2C98 *dst, Vec088B2C98 *src);

void func_088B2C98(Triple088B2C98 *self)
{
    func_088B2B14(&self->third);
    for (int i = 0; i < 2; i++) {
        func_088B2B24(&self->third, &self->pair[i]);
    }
}

}
