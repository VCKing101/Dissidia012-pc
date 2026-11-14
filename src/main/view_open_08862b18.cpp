/* Syncs the manager, then opens a window for the resolved handle through view virtual 10. */
struct View08862B18 {
    char pad_000[0x154];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void open(int handle, int a, int b, int c, int d, int e);
};

struct Manager08862B18 {
    int field_00;
    View08862B18 *view;
};

extern "C" {

extern Manager08862B18 *D_089D5EA0;

void func_088630C8(void);
int func_08867984(int id, int flags);

void func_08862B18(int id, int a, int b, int c, int d)
{
    func_088630C8();
    int handle = func_08867984(id, 0);
    D_089D5EA0->view->open(handle, a, b, c, d, 0);
}

}
