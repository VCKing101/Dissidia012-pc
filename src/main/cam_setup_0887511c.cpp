/* Sets up a camera from its eye and target with the given width and divisor. */
struct Vec4_0887511C {
    float x;
    float y;
    float z;
    float w;
};

struct Cam0887511C {
    Vec4_0887511C base;
    Vec4_0887511C eye;
    Vec4_0887511C target;
    float field_30;
    float field_34;
    float width;
    float field_3C;
    float scale;

    static float inverse(int n)
    {
        if (n != 0) {
            return 1.0f / n;
        }
        return 1.0f;
    }
};

extern "C" {

void func_089A3868(Vec4_0887511C *dst, void *src);

void func_0887511C(Cam0887511C *c, Vec4_0887511C *target, int width, int divisor)
{
    func_089A3868(&c->eye, c);
    func_089A3868(&c->target, target);
    float w = width;
    c->field_34 = c->field_30;
    c->field_3C = 0.0f;
    c->width = w;
    c->scale = Cam0887511C::inverse(divisor);
}

}
