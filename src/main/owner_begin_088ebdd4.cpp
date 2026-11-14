struct Owner;
struct Handler {
    virtual void v0();
    virtual void attach(Owner *o);   /* 0x10 */
    virtual void v2();
    virtual void start(Owner *o);    /* 0x20 */
};
struct Fade { char data[0x24]; };
struct Owner {
    char data_00[0x24];
    int mode;           /* 0x24 */
    Handler *handler;   /* 0x28 */
    Fade fade;          /* 0x2C */
    unsigned char busy; /* 0x50 */
    unsigned char field_51;
    unsigned char field_52;
    unsigned char field_53;
};
extern "C" {
void func_08883F64(void);
void func_08883F78(void);
void func_0885CB80(void (*fn)(Owner *o), Owner *o);
void func_088EC218(Owner *o);
void func_088EB0FC(float start, float end, Fade *f, int from, int to, int x, int y);

void func_088EBDD4(Owner *o, Handler *h, unsigned char instant)
{
    o->busy = 0;
    o->field_51 = 0;
    o->field_53 = 0;
    o->handler = h;
    h->attach(o);
    func_08883F64();
    func_08883F78();
    func_0885CB80(func_088EC218, o);
    if (instant == 1) {
        func_088EB0FC(0.5f, 0.02f, &o->fade, 0x5A, 0, 0x96, 0x3C);
        o->mode = 0;
    } else {
        o->handler->start(o);
        o->busy = 1;
        o->mode = 1;
    }
}
}
