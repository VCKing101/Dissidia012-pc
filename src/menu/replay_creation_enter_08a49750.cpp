/* Collection > Creation replay screen enter: builds the menu's heap block, the 0xBF8-byte state object with its text and a 0x40000 buffer, then starts the load set-up and the task. */
struct Obj4C {
    int buf;
    int w4;
    int w8;
    char p0[0x88C - 0xC];
    int text;
    int slots[18];
    unsigned short uids[18];
    char p1[0xA7C - 0x8FC];
    int a7c;
    char p2[0xBE4 - 0xA80];
    int be4;
    int be8;
};
struct Menu4C {
    char p0[0x100];
    char task[0x38];
    int heap;
    char block[0x10];
    Obj4C *load;
    void Clear() {
        for (int i = 0; i < 18; i++) {
            load->slots[i] = 0;
            load->uids[i] = 0xFFFF;
        }
    }
};
extern "C" {
extern int D_089D7830;
extern int D_089D6998;
extern int D_089D69E8;
extern char D_08A4C0C0[];
extern char D_08A4C010[];
extern char D_08A4C0E8[];
extern char D_08A4C034[];
void func_08A49C58(Menu4C *menu);
void func_08A49D7C(Menu4C *menu);
void func_08A49D28(void);
void func_08A4AA84(Menu4C *menu);
void func_088D00FC(int a, int heap, int size, int b, char *name, int c);
void func_088D52AC(void *block, int heap, int size);
void func_088D5318(void *block, int a, int size);
void func_088D53BC(void *block);
void func_088D1604(int a, int b);
Obj4C *func_0885C6CC(int heap, int size, int a, int b, int c, int d);
void *func_0887B5A8(int size);
void func_0887B1E4(void *p);
void func_0887B260(void *p, char *name, int heap, int a);
void func_0887E630(int a, int buf, int size);
void func_088850EC(int a, int b, char *name);
void func_08885340(int a, int b);
void func_088854C4(int a, int b, int c);
void func_08885388(int a);
void func_088D38F0(void *p);
void func_088D3B5C(void *p, int n);
void func_08865088(void *task, int a, int b, void (*update)(void), char *name);
void func_08883F78(void);
void func_08883F64(void);
}

extern "C" void func_08A49750(Menu4C *menu)
{
    void *none;
    void *part;
    void *made;
    void *keep[1];
    int one;
    int t;
    func_08A49C58(menu);
    int c20 = 0x20;
    func_088D00FC(D_089D7830, menu->heap, 0x180000, c20, D_08A4C0C0, 0);
    func_088D52AC(menu->block, menu->heap, 0xE1000);
    func_088D5318(menu->block, 0x7E, 0xE1000);
    func_088D53BC(menu->block);
    func_088D1604(D_089D7830, 0);
    int c1 = 1;
    menu->load = func_0885C6CC(menu->heap, 0xBF8, 0, 0, c1, 0);
    menu->load->a7c = 0;
    menu->load->be4 = 0;
    Obj4C *l8 = menu->load;
    none = 0;
    keep[0] = none;
    l8->be8 = 0;
    part = func_0887B5A8(8);
    made = keep[0];
    if (part != 0) {
        func_0887B1E4(part);
        made = part;
    }
    menu->load->text = (int)made;
    func_0887B260((void *)menu->load->text, D_08A4C010, menu->heap, 0);
    menu->Clear();
    one = 1;
    int size = 0x40000;
    menu->load = menu->load;
    menu->load->buf = (int)func_0885C6CC(menu->heap, size, 0, 0, one, 0);
    func_0887E630(D_089D6998, menu->load->buf, size);
    menu->load->w4 = 0;
    menu->load->w8 = 0;
    func_08A49D7C(menu);
    t = 0x20;
    func_088850EC(D_089D69E8, t, D_08A4C0E8);
    func_08885340(D_089D69E8, one);
    int code = 0x7C06;
    func_088854C4(D_089D69E8, t, code);
    func_08885388(D_089D69E8);
    func_088D38F0((char *)menu->load + 0xC);
    func_088D3B5C((char *)menu->load + 0x40C, 0x120);
    func_08865088(menu->task, 0, 0, func_08A49D28, D_08A4C034);
    func_08883F78();
    func_08883F64();
    func_08A4AA84(menu);
}
