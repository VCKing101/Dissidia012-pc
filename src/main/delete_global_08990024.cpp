/* Deletes the object held in the global D_089DA420, then clears the global. */
struct Obj08990024 {
    char data[0x904];
    virtual ~Obj08990024() {}
};
extern "C" Obj08990024 *D_089DA420;
extern "C" void func_08990024(void) {
    if (D_089DA420) {
        delete D_089DA420;
        D_089DA420 = 0;
    }
}
