/* Applies a mode setup: announces it, then either loads both side blocks with
   the stored values or starts the current game mode. */
extern "C" {

struct SideBlock {
    char pad_000[6];
    short value;
    char pad_008[4];
    int word;
    char pad_010[0x778 - 0x10];
};

struct VersusSide {
    SideBlock block;
    char pad_778[0x1060 - 0x778];
};

struct VersusState {
    VersusSide sides[2];
    char pad_20C0[0x25AC - 0x20C0];
    short timer;
    short pad_25AE;
    signed char mode;
};

struct ModeSetup {
    int row;
    int busy;
    int a;
    int b;
    int c;
    int d;
    unsigned char ready;
    char pad_19[0x25 - 0x19];
    unsigned char kind;
    char pad_26[0x24 + 0x778 - 0x26];
    int e;
    int f;
};

struct Owner {
    char pad_00[0x18];
    int value;
};

extern VersusState D_09AFB8A0;
extern Owner *D_089D8140;

int func_08887928(ModeSetup *self);
int func_08887A30(ModeSetup *self, int kind);
short func_0888795C(ModeSetup *self);
void func_088F9F88(int on, int a, int b, int c, int d, int e);
void func_088F8E14(VersusSide *side, SideBlock *block);
int func_08900E60(Owner *owner);
void func_088F9B74(int value, int first);

void func_08887730(ModeSetup *self)
{
    VersusState *g = &D_09AFB8A0;
    int a = func_08887928(self);
    int b = func_08887A30(self, self->kind);
    short timer = func_0888795C(self);
    int one = 1;

    func_088F9F88(1, a, b, -1, -1, 0);
    if (self->busy == 0) {
        SideBlock first = *(SideBlock *)((char *)self + 0x24);
        first.word = self->e;
        first.value = self->f;
        func_088F8E14(&g->sides[0], &first);
        VersusSide *sides = g->sides;
        SideBlock second = sides[1].block;
        second.word = self->c;
        second.value = self->d;
        func_088F8E14(&g->sides[1], &second);
    } else {
        signed char mode = g->mode;
        int first = mode == 0;
        int value = 0;
        if (mode == 5) {
            Owner *owner = D_089D8140;
            if (func_08900E60(owner) == one) {
                value = owner->value;
            }
        }
        func_088F9B74(value, first);
    }
    g->timer = timer;
}

}
