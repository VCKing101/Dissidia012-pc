/* Collection > Creation replay list's step: handles the list's pad input, starts the REPLAY.BIN LISTLOAD or SIZES request and finishes the load, delete and copy prompts. */
struct Pad {
    char data[0x34];
    virtual void v0();
    virtual int Pressed(unsigned long long buttons);
};
extern "C" int func_088B1440(int a);
extern "C" int D_089D727C;
extern "C" unsigned char *D_089D7280;
struct Pair {
    unsigned lo;
    unsigned hi;
    static bool Okc()
    {
        if (*D_089D7280 != 0) {
            return true;
        }
        int v = func_088B1440(D_089D727C);
        return v != 0 && v != 0x80110005;
    }
    static bool Invalid(const Pair &p) { unsigned v = p.lo & p.hi; if (v == 0xFFFFFFFF) { return true; } return false; }
};
struct Load {
    char p0[0xC];
    char l0c[0x400];
    int h40c;
    char p1[0x88C - 0x410];
    int obj88c;
    char t890;
    char p2[0xA7C - 0x891];
    int a7c;
    unsigned flags;
    int a84;
    int kind;
    char p3[0xAB4 - 0xA8C];
    int ab4;
    char p4[0xADC - 0xAB8];
    int adc;
    char p5[0xBE3 - 0xAE0];
    unsigned char be3;
    static unsigned char Has(unsigned f, unsigned m) { return (f & m) != 0; }
};
struct Menu {
    char p0[0x13C];
    char l13c[0x10];
    Load *load;
};
extern "C" {
extern int D_089D7260;
extern int D_089D7F90;
extern struct { char p[8]; char *part; } *D_089D8530;
extern unsigned long long D_08A4C090;
extern unsigned long long D_08A4C088;
extern char D_08A4BE74[];
extern char D_08A4BED4[];
Pad *func_088729A0(int port);
void func_088729C0(void);
void func_08872A18(void);
void func_08873D34(int mode, void (*cb)(void), void *owner, int d, int e, float time);
void func_08873DB0(float *color, int mode);
int func_0887B500(int obj, int a, unsigned v);
void func_088AEFA0(int mgr);
int func_088AF0EC(int mgr);
void func_088AF2AC(int mgr);
void func_088AF348(int mgr);
void func_088B1230(int a, int b, int c, int d, int e, int f);
int func_088B1440(int a);
void func_088B16FC(unsigned char *p);
void func_088B1764(unsigned char *p);
int func_088B178C(unsigned char *p);
void func_088D3F00(void *a, void *b, char *c);
void func_088D44EC(void *a, void *b, char *c);
void func_088D4BF4(int a);
int func_088D4C5C(void);
void func_088D54DC(void *p);
int func_088D688C(void *p, int a);
void func_088D9F5C(int h, int a, int b);
void func_088FCB8C(int a);
void func_088FF914(int a);
void func_08929958(Pair *out, char *a);
void func_08929A70(Pair *out, char *a);
void func_08A49D7C(Menu *menu);
int func_08A49E8C(Menu *menu);
void func_08A49F1C(Menu *menu);
void func_08A4A270(Menu *menu);
void func_08A4A3C4(Menu *menu);
void func_08A4A5C8(Menu *menu, int a);
int func_08A4A6FC(Menu *menu);
void func_08A4AA48(Menu *menu);
int func_08A4AB60(Menu *menu);
void func_08A4ACE0(void);
void func_08A4ACFC(Menu *menu);
int func_08A4B250(Menu *menu);

extern "C" void func_08A4B5AC(Menu *menu)
{
    int s1;
    unsigned f;
    int done;
    int go;
    int ok;
    int r;
    unsigned lo1;
    unsigned lo2;
    char *pp;
    Pair v1;
    Pair tmp1;
    Pair v2;
    Pair tmp2;
    float col[4];
    float tm;
    func_088D3F00(menu->load->l0c, &menu->load->h40c, D_08A4BE74);
    func_088D44EC(&menu->load->h40c, &menu->load->t890, D_08A4BED4);
    f = menu->load->flags;
    s1 = 0;
    if (f & 1) {
        if (Load::Has(f, 4)) {
            menu->load->a7c = 3;
        }
    } else if (f & 0x10) {
        r = func_08A4B250(menu);
        while (1) {
            if (r != 1) {
                unsigned nf;
                Load *l;
                if (r == 2) {
                    func_088FF914(menu->load->ab4);
                    menu->load->adc = menu->load->kind;
                    s1 = 1;
                    l = menu->load;
                    nf = l->flags & ~0x10u;
                } else {
                    l = menu->load;
                    nf = l->flags & ~0x10u;
                }
                l->flags = nf;
            }
            break;
        }
    } else if (func_08A4A6FC(menu) == 0) {
        f = menu->load->flags;
        if (f & 8) {
            if (*D_089D7280 == 0) {
                if (menu->load->kind == 4) {
                    if (func_088AF0EC(D_089D7260) == 0) {
                        s1 = 1;
                        menu->load->adc = menu->load->kind;
                    }
                } else if (menu->load->kind == 5) {
                    if (func_088AF0EC(D_089D7260) == 0) {
                        if (func_08A49E8C(menu)) {
                            func_08A4A5C8(menu, 0xA8);
                            func_088D4BF4(4);
                        } else {
                            func_08A4ACFC(menu);
                            func_088D9F5C(menu->load->h40c, 0x53, 1);
                            menu->load->flags |= 0x10;
                        }
                    }
                } else if (menu->load->kind == 1) {
                    if (func_088AF0EC(D_089D7260) == 0) {
                        menu->load->adc = menu->load->kind;
                        pp = D_089D8530->part + 0x2C90;
                        func_08929958(&tmp1, pp);
                        lo1 = tmp1.lo;
                        v1.lo = lo1;
                        v1.hi = tmp1.hi;
                        ok = Pair::Invalid(v1);
                        while (1) {
                            if (!ok) {
                                menu->load->flags |= 0x20;
                                func_088729C0();
                                func_088B16FC(D_089D7280);
                                r = func_0887B500(menu->load->obj88c, 8, lo1);
                                func_088B1230(D_089D727C, r, 0, 0, 0xA0, 0);
                            } else {
                                s1 = 1;
                            }
                            break;
                        }
                    }
                } else if (menu->load->kind == 2) {
                    if (func_088AF0EC(D_089D7260) == 0) {
                        menu->load->adc = menu->load->kind;
                        pp = D_089D8530->part + 0x2C90;
                        func_08929A70(&tmp2, pp);
                        lo2 = tmp2.lo;
                        v2.lo = lo2;
                        v2.hi = tmp2.hi;
                        ok = Pair::Invalid(v2);
                        while (1) {
                            if (!ok) {
                                menu->load->flags |= 0x20;
                                func_088729C0();
                                func_088B16FC(D_089D7280);
                                r = func_0887B500(menu->load->obj88c, 8, lo2);
                                func_088B1230(D_089D727C, r, 0, 0, 0xA0, 0);
                            } else {
                                s1 = 1;
                            }
                            break;
                        }
                    }
                }
                menu->load->flags &= ~8u;
                menu->load->flags |= 0x1000;
            }
        } else if (f & 0x20) {
            ok = Pair::Okc();
            if (!ok) {
                menu->load->flags &= ~0x20u;
                menu->load->flags |= 0x1000;
                func_08872A18();
                func_088B1764(D_089D7280);
            }
        } else if (f & 0x1000) {
            if (func_088B178C(D_089D7280) == 0) {
                menu->load->flags &= ~0x1000u;
            }
        } else {
            func_088FCB8C(D_089D7F90);
            if (func_08A4AB60(menu) == 0) {
                if (func_088729A0(0)->Pressed(D_08A4C090)) {
                    done = 1;
                    if (menu->load->kind == 4 || menu->load->kind == 5 || menu->load->kind == 6) {
                        func_08A49D7C(menu);
                        done = 0;
                        go = 1;
                        if (menu->load->kind == 5) {
                            if (func_08A49E8C(menu)) {
                                func_08A4A5C8(menu, 0xA8);
                                func_088D4BF4(4);
                                go = 0;
                            }
                        }
                        while (1) {
                            if (go) {
                                func_088AEFA0(D_089D7260);
                                switch (menu->load->kind) {
                                case 4:
                                case 5:
                                    func_088AF2AC(D_089D7260);
                                    break;
                                case 6:
                                    func_088AF348(D_089D7260);
                                    break;
                                }
                                menu->load->flags |= 8;
                                func_088D4BF4(1);
                            }
                            break;
                        }
                    } else if (menu->load->kind == 1 || menu->load->kind == 2 || menu->load->kind == 3) {
                        func_08A49F1C(menu);
                        done = 0;
                        func_088AEFA0(D_089D7260);
                        switch (menu->load->kind) {
                        case 1:
                            menu->load->be3 = 1;
                        case 2:
                            func_088AF2AC(D_089D7260);
                            menu->load->flags |= 8;
                            break;
                        case 3:
                            func_088AF348(D_089D7260);
                            menu->load->flags |= 8;
                            break;
                        }
                        func_088D4BF4(1);
                    } else if (menu->load->kind == 0) {
                        func_08A4AA48(menu);
                    }
                    while (1) {
                        if (done) {
                            func_088D4BF4(1);
                            s1 = 1;
                            menu->load->adc = menu->load->kind;
                        }
                        break;
                    }
                } else if (func_088729A0(0)->Pressed(D_08A4C088)) {
                    func_088D4BF4(2);
                    menu->load->adc = -1;
                    s1 = 1;
                } else if (func_088D688C(&menu->load->a84, 0)) {
                    func_08A4A270(menu);
                }
            }
        }
    }
    if (s1) {
        menu->load->flags |= 1;
        func_088D54DC(menu->l13c);
        tm = (float)func_088D4C5C();
        func_08873D34(3, func_08A4ACE0, 0, 1, 2, tm);
        col[0] = 255.0f;
        col[1] = 255.0f;
        col[2] = 255.0f;
        col[3] = 255.0f;
        func_08873DB0(col, 2);
    }
    func_08A4A3C4(menu);
}
}
