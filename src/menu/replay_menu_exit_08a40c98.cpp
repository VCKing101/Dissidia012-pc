/* The other replay menu's exit: hands the chosen replay to the game (FIELD mode 9 then 0x08889BAC), frees the menu's
   objects and sets what happens next (a message, the title, the battle or the previous menu). */
extern "C" {
extern int D_089D7830;
extern int D_089D7260;
extern void *D_089D6A10;
extern int D_089E75C4;
extern unsigned char D_09B6DA68[];
extern int func_088D4C5C(void);
extern void func_088D5528(void *p, int heap);
extern void func_088D0370(int a, int b, int c);
extern void func_08A3C44C(void *p, int n);
extern void func_08A42EF0(void *dst, void *src);
extern int func_08A42F64(void *p);
extern void func_08A42FC0(void *p);
extern void func_08873D34(float f, int a, int b, int c, int d, int e);
extern void func_08873DEC(int a, int b);
extern void func_08A41244(void *menu);
extern void func_0888964C(void *field, int mode, int a, int b, int c);
extern void func_08889BAC(void *field, char *save, bool on);
extern void func_08889808(void *field);
extern void func_088AF044(int manager, char *save);
extern void func_088AEF98(int manager);
extern void func_0885C538(int heap, void *p, int a);
extern void func_08A42AD0(void *menu);
extern void func_08A4D678(void);
extern void func_089E0BDC(int a, int b);
}
struct ReplayLoad {
    void *unk_00;
    char p0[0xE3C - 4];
    char *save;
    char p1[0xE94 - 0xE40];
    char buffer[1];
    char p2[0x1296 - 0xE95];
    unsigned char kind;
    char p3[3];
    unsigned char go;
};
struct ReplayMenu {
    char p0[0x134];
    int heap;
    char p1[0x14C - 0x138];
    ReplayLoad *load;
    void *other;
};
extern "C" void func_08A40C98(ReplayMenu *menu)
{
    char buf[0x40];
    func_088D5528((char *)menu + 0x13C, menu->heap);
    int t = 0x11;
    func_088D0370(D_089D7830, t, 1);
    *(int *)(D_089D7830 + 0x10964) = 0;
    *(int *)(D_089D7830 + 0x10968) = 0;
    *(int *)(D_089D7830 + 0x1096C) = 0;
    if (D_09B6DA68[0x1E]) {
        func_08A3C44C(menu->load->buffer, 0x200);
        func_08A42EF0(buf, menu->load->buffer);
        if (func_08A42F64(menu->load->buffer)) {
            func_08873D34((float)func_088D4C5C(), 2, 0, 0, 1, 2);
            func_08873DEC(0, 2);
        }
    }
    func_08A41244(menu);
    D_09B6DA68[0x1C] = 0;
    unsigned char go = menu->load->go;
    if (go) {
        if (menu->load->save) {
            char *p = menu->load->save;
            func_0888964C(D_089D6A10, 9, 0, 0, 0);
            func_08889BAC(D_089D6A10, p + 0x2C90, menu->load->kind == 2);
        } else {
            go = 0;
        }
        D_09B6DA68[0x1C] = menu->load->kind;
    }
    char *save = menu->load->save;
    if (save) {
        func_088AF044(D_089D7260, save);
        func_0885C538(menu->heap, menu->load->save, 0);
        menu->load->save = 0;
    }
    func_088AEF98(D_089D7260);
    func_0885C538(menu->heap, menu->load, 0);
    menu->load = 0;
    void *o = menu->other;
    func_0885C538(menu->heap, o, 0);
    menu->other = 0;
    func_08A42AD0(menu);
    if (D_09B6DA68[0x1E]) {
        func_08A42FC0(buf);
        D_09B6DA68[0x1E] = 0;
    } else if (D_09B6DA68[0x1F]) {
        func_08A4D678();
        D_09B6DA68[0x1F] = 0;
    } else if (go) {
        func_08889808(D_089D6A10);
    } else {
        int g = D_089E75C4; int z = 0; func_089E0BDC(g, z);
    }
}
