extern "C" {
struct FxObjectBase {
    char pad_00[0x24];
    virtual void finish();
};
struct FxObject : FxObjectBase {
    void *handle;               /* 0x28 */
    void *item;                 /* 0x2C */
};
extern void func_0892ACAC(FxObject *o);
extern void func_0892E1B4(FxObject *o);
extern void func_0892E0C8(FxObject *o);
extern void func_088DAB44(void *item);
void func_0892DBE4(FxObject *o)
{
    func_0892ACAC(o);
    func_0892E1B4(o);
    func_0892E0C8(o);
    if (o->item != 0) {
        func_088DAB44(o->item);
        o->item = 0;
    }
    o->finish();
}
}
