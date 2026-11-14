struct Heaps08881F2C {
    unsigned char unknown_000000[0xECDC00];
    int heap_a;
    int heap_b;

    static Heaps08881F2C *init(Heaps08881F2C *self) {
        self->heap_a = 0;
        self->heap_b = 0;
        return self;
    }
};

extern "C" {
Heaps08881F2C *func_08881F2C(Heaps08881F2C *self) {
    return Heaps08881F2C::init(self);
}
}
