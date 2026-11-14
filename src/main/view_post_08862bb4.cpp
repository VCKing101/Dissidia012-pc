/* Syncs the manager, then forwards five arguments to view virtual 11. */
struct View08862BB4 {
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
    virtual void v10();
    virtual void post(int a, int b, int c, int d, int e);
};

struct Manager08862BB4 {
    int field_00;
    View08862BB4 *view;
};

extern "C" {

extern Manager08862BB4 *D_089D5EA0;

void func_088630C8(void);

void func_08862BB4(int a, int b, int c, int d, int e)
{
    func_088630C8();
    D_089D5EA0->view->post(a, b, c, d, e);
}

}
