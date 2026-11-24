/* Starts a mode round: resets the mode, then either closes an open menu or
   clears each unit's hold bit and sends the round-start message. */
typedef struct UnitFlags {
    unsigned char b[7];
    unsigned char bits;
} UnitFlags;

typedef struct ModeUnit {
    char pad_000[0x380];
    unsigned int status;
    char pad_384[0x5B0 - 0x384];
    UnitFlags flags;
} ModeUnit;

typedef struct ModeCtl {
    char pad_00[0x38];
    ModeUnit *units[4];
    int state;
    int kind;
} ModeCtl;

typedef struct MenuInfo {
    char pad_00[0x94];
    int is_open;
} MenuInfo;

typedef struct Message {
    int kind;
    int id;
    char pad_08;
    unsigned char flag;
    char pad_0A[0x30 - 0xA];
} Message;

extern int D_089D69F8;
extern MenuInfo *D_08B11E28;
extern int D_08B1FE60;
extern int D_08ABBDF8;
extern unsigned char *D_08B212E8;

extern void func_08886C88(int p);
extern void func_08B0DE14(MenuInfo *menu);
extern void func_08B158C8(void);
extern int func_08B1DBD0(int p);
extern void func_08B0DDC8(MenuInfo *menu, int flag);
extern void func_08ABB4BC(int p);
extern void func_089EB798(Message *m);
extern void func_0888ACE4(Message *m);
extern void func_089EB7D0(Message *m, int flags);

void func_08888978(ModeCtl *self)
{
    int one;
    int active;
    int i;

    func_08886C88(D_089D69F8);
    func_08B0DE14(D_08B11E28);
    func_08B158C8();
    active = func_08B1DBD0(D_08B1FE60);
    one = 1;
    if (active == one) {
        func_08B0DDC8(D_08B11E28, 1);
        self->state = 3;
    } else if (!(unsigned char)(D_08B11E28->is_open == 1)) {
    for (i = 0; i < 2; i++) {
        if (self->units[i] != 0) {
            self->units[i]->status &= ~2;
        }
    }
    func_08ABB4BC(D_08ABBDF8);
    self->state = 3;
    if (self->kind == 5) {
        *D_08B212E8 = one;
    }
    {
        UnitFlags *f = &self->units[1]->flags;
        if ((unsigned char)((f->bits & 0x10) ? 1 : 0) == one) {
            Message m;
            func_089EB798(&m);
            m.kind = 0;
            m.id = 0x9B;
            m.flag = 1;
            func_0888ACE4(&m);
            func_089EB7D0(&m, 2);
        }
    }
    }
}
