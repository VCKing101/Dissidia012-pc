/* The replay list's exit: in states 4 and 5 starts the replay (FIELD mode 9, then the hand-off with the save object),
   frees the menu's objects and dispatches on the state. */
extern "C" {
extern int D_089D7830;
extern int D_089D6998;
extern void *D_089D6A10;
extern int D_09B6DD08;
extern void func_08A4ABA0(void *menu);
extern void func_08A49F7C(void *menu);
extern void func_08A49E10(void *menu);
extern void func_08A49CB0(void *menu);
extern void func_08A4D57C(int b);
extern void func_08A50134(int a);
extern void func_08A4AC94(void);
extern void func_0888964C(void *field, int mode, int a, int b, int c);
extern void func_08889BAC(void *field, char *save, int on);
extern void func_08889808(void *field);
extern void func_0887B1F4(int obj, int a);
extern void func_088D5528(void *p, int heap);
extern void func_088D0370(int a, int b, int c);
extern void func_08883F8C(void);
extern void func_0887E684(int a);
extern void func_0885C538(int heap, void *p, int a);
}
struct ReplayLoad {
    void *unk_00;
    char *save;
    char p0[0x88C - 8];
    int obj;
    char p1[0xADC - 0x890];
    int state;
    char p2[0xBE3 - 0xAE0];
    unsigned char byte;
};
struct ReplayMenu {
    char p0[0x138];
    int heap;
    char p1[0x14C - 0x13C];
    ReplayLoad *load;
};
extern "C" void func_08A499A0(ReplayMenu *menu)
{
    func_08A4ABA0(menu);
    int state = menu->load->state;
    if (state == 4 || state == 5) {
        char *save = menu->load->save;
        func_0888964C(D_089D6A10, 9, 0, 0, 0);
        int v = 0;
        if (state == 5) v = 1;
        func_08889BAC(D_089D6A10, save + 0x2C90, v);
    }
    int b = menu->load->byte;
    func_08A49F7C(menu);
    func_08A49E10(menu);
    if (menu->load->obj != 0) {
        func_0887B1F4(menu->load->obj, 3);
        menu->load->obj = 0;
    }
    func_088D5528((char *)menu + 0x13C, menu->heap);
    int t = 0x20;
    func_088D0370(D_089D7830, t, 1); ;
    func_08883F8C();
    func_0887E684(D_089D6998);
    func_0885C538(menu->heap, menu->load->unk_00, 0);
    menu->load->unk_00 = 0;
    void *p = menu->load;
    func_0885C538(menu->heap, p, 0);
    menu->load = 0;
    func_08A49CB0(menu);
    switch (state) {
    case 0:
    case 1:
    case 3:
        D_09B6DD08 = state;
        func_08A4D57C(b);
        break;
    case 2:
        D_09B6DD08 = state;
        func_08A50134(2);
        break;
    case 4:
    case 5:
    case 6:
        D_09B6DD08 = state;
        func_08889808(D_089D6A10);
        break;
    default:
        D_09B6DD08 = 0;
        func_08A4AC94();
        break;
    }
}
