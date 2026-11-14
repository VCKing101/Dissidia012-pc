/* Clears the owner: both sub-objects, both item lists and the id pairs with their names. */
struct Pair088BF41C {
    int id;
    void *value;
};

struct Owner088BF41C {
    char pad_00[0x5D8];
    Pair088BF41C *pairs;
    int count;
    void *names;
};

extern "C" {
void func_088BD564(void *sub);
void func_088B1F38(void *p);
void func_088BD3C8(void *p, int flags);
void func_088BD99C(void *p);
void func_088BF41C(Owner088BF41C *self)
{
    func_088BD564(self);
    func_088BD564((char *)self + 0x40);
    func_088B1F38((char *)self + 0x80);
    func_088B1F38((char *)self + 0x32C);
    if (self->pairs != 0) {
        for (int i = 0; i < self->count; i++) {
            func_088BD3C8(self->pairs[i].value, 2);
        }
        func_088BD99C(self->names);
        self->names = 0;
        func_088BD99C(self->pairs);
        self->pairs = 0;
        self->count = 0;
    }
}
}
