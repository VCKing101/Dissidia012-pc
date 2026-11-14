struct Record1EE1C {
    unsigned short flags;
    unsigned short id;
    char pad_04[0x24C - 4];
};

struct Work1EE1C {
    char pad_0000[0x2BC0];
    Record1EE1C records[26];
    char pad_6778[4];
    short order[26];
    char pad_67B0[0x6964 - 0x67B0];
    int object;
};

struct Screen1EE1C {
    char pad_00[0x154];
    Work1EE1C *work;
};

extern "C" {
extern int D_089D712C;
extern int func_08A1D8E4(Screen1EE1C *self);
extern void func_088F7C70(int object, int id);
extern void func_088F5484(int save, int id);
extern void func_088ACA10(int save);
}

extern "C" void func_08A1EE1C(Screen1EE1C *self, int index)
{
    int id = self->work->records[self->work->order[index]].id;
    int save;
    if (func_08A1D8E4(self) != 0) {
        func_088F7C70(self->work->object, id);
        save = D_089D712C;
    } else {
        func_088F5484(D_089D712C + 0x1FECC, id);
        save = D_089D712C;
    }
    func_088ACA10(save);
    self->work->records[id].flags |= 4;
}
