/* Syncs the manager, then forwards six arguments to view virtual 8. */
struct View08862F2C {
    char pad_000[0x154];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void send(int a, int b, int c, int d, int e, unsigned char f);
};

struct Manager08862F2C {
    int field_00;
    View08862F2C *view;
};

extern "C" {

extern Manager08862F2C *D_089D5EA0;

void func_088630C8(void);

void func_08862F2C(int a, int b, int c, int d, int e, unsigned char f)
{
    func_088630C8();
    D_089D5EA0->view->send(a, b, c, d, e, f);
}

}
