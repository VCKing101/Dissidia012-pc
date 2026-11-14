struct Models46180 {
    int pad_00;
    int first;
    int second;
};

struct Layout46180 {
    int pad_00;
    Models46180 *models;
};

struct Window46180 {
    char pad_000[0x418];
    Layout46180 *layout;
};

struct Pair46180 {
    unsigned short first;
    unsigned short second;
};

extern "C" {
extern Pair46180 D_08A48098[];
extern void func_088D9F5C(int model, unsigned short id, int flag);
}

extern "C" void func_08A46180(Window46180 *self, int index)
{
    int model = self->layout->models->first;
    Pair46180 *pair = &D_08A48098[index];
    func_088D9F5C(model, pair->first, 1);
    func_088D9F5C(self->layout->models->second, pair->second, 1);
}
