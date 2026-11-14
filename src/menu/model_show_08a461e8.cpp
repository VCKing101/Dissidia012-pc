struct Models461E8 {
    int pad_00[5];
    int slots[1];
};

struct Layout461E8 {
    int pad_00;
    int *models;
};

struct Window461E8 {
    char pad_000[0x418];
    Layout461E8 *layout;
};

extern "C" {
extern unsigned short D_08A480AC[];
extern void func_088D9F5C(int model, unsigned short id, int flag);
}

extern "C" void func_08A461E8(Window461E8 *self, int slot, int row, int column)
{
    func_088D9F5C(self->layout->models[slot + 5], D_08A480AC[row * 3 + column], 1);
}
