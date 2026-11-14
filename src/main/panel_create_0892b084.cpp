extern "C" {
typedef struct Panel { int field_00; void *handle; } Panel;
extern void *D_089D7830;
extern void func_0892B6FC(Panel *panel);
extern void *func_088D0700(void *manager, unsigned char kind, int a, int b);
struct PanelOps {
    static void *create(unsigned char kind, int a, int b) { return func_088D0700(D_089D7830, kind, a, b); }
};
void func_0892B084(Panel *panel, int kind, int a, int b)
{
    func_0892B6FC(panel);
    panel->handle = PanelOps::create(kind, a, b);
}
}
