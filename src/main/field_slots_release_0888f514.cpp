/* Releases the menu slot of each active field slot and sends the release
   message (0x4E) for that side. */
extern "C" {

struct FieldSlot {
    void *unit;
    unsigned char used;
    unsigned char active;
    char pad_06[2];
};

struct FieldCtl {
    char pad_00[4];
    FieldSlot slots[2];
};

struct Message {
    int kind;
    int id;
    char pad_08;
    unsigned char flag;
    char pad_0A[0x30 - 0xA];
};

extern int D_08A22AB0;

void func_08A22328(int menu, int slot);
void func_089EB798(Message *m);
void func_0888ACE4(Message *m);
void func_089EB7D0(Message *m, int flags);

void func_0888F514(FieldCtl *self)
{
    int menu = D_08A22AB0;

    for (int i = 0; i < 2; i++) {
        FieldSlot *slot = &self->slots[i];
        if (slot->active == 1) {
            Message m;
            func_08A22328(menu, i);
            func_089EB798(&m);
            m.id = 0x4E;
            m.flag = 1;
            m.kind = i;
            func_0888ACE4(&m);
            func_089EB7D0(&m, 2);
        }
    }
}

}
