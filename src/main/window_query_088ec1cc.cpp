struct WindowListener {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void opened(void *window);
    virtual void v04();
    virtual unsigned char query(void *window);
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
void func_088EC1CC(WindowState *w)
{
    if (w->listener != 0) {
        w->result = w->listener->query(w);
    }
}
}
