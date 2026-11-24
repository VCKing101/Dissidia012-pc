/* Stores the field's start area and sends the area message (0x4B) for both
   sides. */
extern "C" {
struct AreaInfo {
    char pad_00[4];
    short x;
    char pad_06[2];
    short y;
};

struct FieldCtl {
    char pad_00[0xC];
    int x;
    int y;
};

struct Message {
    int kind;
    int id;
    char pad_08;
    unsigned char flag;
    char pad_0A[0x30 - 0xA];
};

extern AreaInfo *D_08AF2BD4;
extern int D_08AE7CAC;
extern int D_08AE7CB0;

void func_089EB798(Message *m);
void func_0888ACE4(Message *m);
void func_089EB7D0(Message *m, int flags);

void func_0888EB5C(FieldCtl *self)
{
    AreaInfo *area = D_08AF2BD4;
    Message m;

    self->x = area->x;
    self->y = area->y;
    D_08AE7CAC = self->x;
    D_08AE7CB0 = self->y;
    func_089EB798(&m);
    m.id = 0x4B;
    m.flag = 1;
    for (int i = 0; i < 2; i++) {
        m.kind = i;
        func_0888ACE4(&m);
    }
    func_089EB7D0(&m, 2);
}

}
