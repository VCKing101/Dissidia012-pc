// If the object from func_088B9084 lacks bit 0 of byte 1, sets bits 0 and 1.
struct Fl {
    unsigned char b0, b1;
    bool has_a() { return (b1 & 1) != 0; }
    void set_a() { b1 |= 1; }
    void set_b() { b1 |= 2; }
};
extern "C" Fl *func_088B9084(void);
extern "C" void func_088B8EB8(void)
{
    Fl *p = func_088B9084();
    if (p) {
        if (!(p->b1 & 1)) {
            p->set_a();
            p->set_b();
        }
    }
}
