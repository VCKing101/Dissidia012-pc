/* Window update: when the opener finishes, notifies the listener and marks it open. */
struct WindowListener {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void opened(void *window);
    virtual void v04();
    virtual int query(void *window);
};

struct WindowState {
    char pad_00[0x24];
    int open;
    WindowListener *listener;
    char opener[0x50 - 0x2C];
    unsigned char notified;
    unsigned char result;
};

extern "C" {
extern int func_088EB17C(void *opener);
extern void func_088EBA98(void);
extern void func_088EB65C(void *opener);

void func_088EC140(WindowState *w)
{
    void *opener = w->opener;

    if (func_088EB17C(opener) == 1) {
        if (w->listener != 0) {
            w->listener->opened(w);
            w->notified = 1;
        }
        w->open = 1;
    }
    func_088EBA98();
    func_088EB65C(opener);
}
}
