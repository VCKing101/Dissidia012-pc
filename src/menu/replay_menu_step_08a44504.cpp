/* The other replay menu's step: handles its busy flags, then on confirm/cancel/page presses builds the 60,000-byte REPLAY.BIN save object, starts the LISTLOAD or SIZES request and frees objects. */
struct Pad {
    char data[0x34];
    virtual void v0();
    virtual int Pressed(unsigned long long buttons);
};
struct Panel {
    int p0;
    int h4;
    char p1[0x4C - 8];
    int h4c;
};
struct Load {
    char p0[0x400];
    Panel panel;
    char p1[0x8AC - 0x450];
    int obj8ac;
    int kind8b0;
    char p2[0xE3C - 0x8B4];
    void *save;
    unsigned flags;
    char p3[0xE54 - 0xE44];
    int e54;
    char p4[0x1296 - 0xE58];
    unsigned char b1296;
    char p5[3];
    unsigned char b129a;
    unsigned char Has(unsigned m) { return (flags & m) != 0; }
    int Pick(int c, int v) { if (v < 0) c = 0xA9; return c; }
};
struct Menu {
    char p0[0x134];
    int heap;
    char p1[0x14C - 0x138];
    Load *load;
};
extern "C" {
extern int D_089D7260;
extern unsigned char *D_089D7280;
extern unsigned long long D_08A45650;
extern unsigned long long D_08A45658;
Pad *func_088729A0(int port);
int func_08A42090(Menu *menu);
void func_08A42B08(Menu *menu);
void func_08A42B7C(Menu *menu);
int func_08A44108(Menu *menu);
int func_08A41DC4(Menu *menu);
void func_08A41F70(Menu *menu, int code);
void func_08A43CB0(Menu *menu);
void func_08A4146C(Menu *menu, int a);
void func_088D9CD8(int h);
void func_088D9CFC(int h);
void func_088D9F5C(int h, int a, int b);
void func_088D4BF4(int a);
void func_088D688C(void *obj, int a);
int func_088FF908(void);
int func_088AF0EC(int mgr);
void func_088AF044(int mgr);
void func_088AEFA0(int mgr);
void func_088AF2AC(int mgr);
void func_088AF348(int mgr);
void func_088AF020(int mgr, int heap);
void func_088B0AF8(void *save);
void func_0885C538(int heap, void *p, int a);
void *func_0885C6CC(int heap, int size, int a, int b, int c, int d);

int func_08A44504(Menu *menu)
{
    Load *load = menu->load;
    Panel *panel = &load->panel;
    unsigned mask;
    int code;
    int one;
    if (load->Has(2)) {
        if (func_08A42090(menu)) {
            menu->load->flags &= ~2u;
            func_08A42B08(menu);
            func_088D9CD8(panel->h4);
        }
    } else if (load->Has(0x800)) {
        if (func_08A44108(menu)) {
            if (menu->load->b129a) return 1;
            func_08A42B08(menu);
            func_088D9CD8(panel->h4);
            menu->load->flags &= ~0x800u;
        }
    } else if (load->Has(0x100)) {
        if (*D_089D7280 == 0) {
            load->b1296 = load->kind8b0;
            mask = ~0x100u;
            if (menu->load->b1296 == 0) {
                if (func_088AF0EC(D_089D7260) == 0) {
                    one = 1;
                    menu->load->b129a = one;
                    menu->load->flags &= mask;
                    return one;
                }
            } else if (menu->load->b1296 == 2) {
                if (func_088AF0EC(D_089D7260) == 0) {
                    if (func_08A41DC4(menu)) {
                        func_08A41F70(menu, 0xA8);
                        menu->load->flags |= 2;
                        menu->load->flags &= mask;
                        func_088D9CFC(panel->h4);
                        return 0;
                    }
                    func_08A43CB0(menu);
                    menu->load->flags &= mask;
                    return 0;
                }
            }
            func_08A42B08(menu);
            func_088D9CD8(panel->h4);
            menu->load->flags &= mask;
        }
    } else if (load->Has(0x1000)) {
        load->flags &= ~0x1000u;
        func_08A42B7C(menu);
        code = 0xAA;
        code = load->Pick(code, func_088FF908());
        func_08A41F70(menu, code);
        menu->load->flags |= 2;
        func_088D9CFC(panel->h4);
        return 0;
    } else {
    if (func_088729A0(0)->Pressed(D_08A45650)) {
        load = menu->load;
        one = 1;
        if (load->save != 0) {
            func_088AF044(D_089D7260);
            func_0885C538(menu->heap, menu->load->save, 0);
            menu->load->save = 0;
            load = menu->load;
        }
        if (load->e54 == 6) {
            func_088D9F5C(panel->h4c, 0x1E, 1);
        } else {
            func_088D9F5C(panel->h4c, 0x1F, 1);
        }
        *(short *)(panel->h4c + 0xE) = 0x1D;
        func_088D4BF4(2);
        func_08A4146C(menu, 0);
        return one;
    }
    if (func_088729A0(0)->Pressed(D_08A45658)) {
        func_08A42B7C(menu);
        if (menu->load->kind8b0 == 2 && func_08A41DC4(menu)) {
            func_08A41F70(menu, 0xA8);
            menu->load->flags |= 2;
            func_088D9CFC(panel->h4);
            return 0;
        }
        load = menu->load;
        if (load->save == 0) {
            int c1 = 1;
            menu->load->save = func_0885C6CC(menu->heap, 60000, 0, 0, c1, 0);
            func_088B0AF8(menu->load->save);
            func_088AF020(D_089D7260, menu->heap);
            load = menu->load;
        }
        if (load->save != 0) {
            func_088AEFA0(D_089D7260);
            switch (menu->load->kind8b0) {
            case 0:
                func_088AF2AC(D_089D7260);
                break;
            case 1:
                func_088AF348(D_089D7260);
                break;
            case 2:
                func_088AF2AC(D_089D7260);
                break;
            }
            func_088D9CFC(panel->h4);
            func_088D4BF4(1);
            menu->load->flags |= 0x100;
        }
    } else {
        func_088D688C(&menu->load->obj8ac, 0);
    }
    }
    return 0;
}
}
