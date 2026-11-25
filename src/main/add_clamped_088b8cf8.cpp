// Adds delta to the signed byte at offset 2 of the object from func_088B8FD0, clamped to 0..99, and sets bit 3 of byte 3.
struct E44b { char b0, b1; signed char v; unsigned char flags; };
extern "C" E44b *func_088B8FD0(void);
struct H {
    static int clamp(int d) { int hi = 99; if (d < 0) return 0; if (hi < d) return hi; return d; }
    static void run(int a, int b, int delta)
    {
        E44b *e = func_088B8FD0();
        if (e) {
            delta = e->v + delta;
            delta = clamp(delta);
            e->v = delta;
            e->flags = e->flags | 8;
        }
    }
};
extern "C" void func_088B8CF8(int a, int b, int delta) { H::run(a, b, delta); }
