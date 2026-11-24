// Returns 1 when the actor (non-null, with a handle) carries one of the marker pairs for its side.
extern "C" {
extern int func_08B3A5BC(void *, int);
extern void *func_08835798(void *);
extern int func_088374B0(void *);
extern int func_088BF4CC(void *, int);
struct Marker {
    static bool Has(void *h, int id) { return func_088BF4CC(h, id) != 0; }
};
unsigned char func_08AB344C(void *self) {
    if (self == 0) return 0;
    if (func_08B3A5BC(self, 0) == 0) return 0;
    void *obj = self;
    void *h = func_08835798(obj);
    if (func_088374B0(obj) == 0) {
        if (!Marker::Has(h, 0x10E) && !Marker::Has(h, 0x110)) return 0;
    } else {
        if (!Marker::Has(h, 0x10F) && !Marker::Has(h, 0x111)) return 0;
    }
    return 1;
}
}
