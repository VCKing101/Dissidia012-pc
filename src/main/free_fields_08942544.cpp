// Free four owned blocks of the record and clear the count.
extern "C" void func_089748D0(int p);
extern "C" void func_08942544(int *self)
{
    int *keep[1];
    int v = self[0];
    if (v != 0) {
        keep[0] = self;
        func_089748D0(v);
        self = keep[0];
        self[0] = 0;
    }
    v = self[2];
    if (v != 0) {
        keep[0] = self;
        func_089748D0(v);
        self = keep[0];
        self[2] = 0;
    }
    v = self[3];
    if (v != 0) {
        keep[0] = self;
        func_089748D0(v);
        self = keep[0];
        self[3] = 0;
    }
    v = self[4];
    if (v != 0) {
        keep[0] = self;
        func_089748D0(v);
        self = keep[0];
        self[4] = 0;
    }
    self[5] = 0;
}
