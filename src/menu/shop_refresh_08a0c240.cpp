/* Refreshes the shop lists of the object in D_08A23FF0 while the object
   in D_089D69E8 is held. The calls on D_089D69E8 are inlined wrappers with
   char parameters (that is what puts the constant before the load). */
extern "C" {
extern void *D_089D69E8;
extern void *D_08A23FF0;
void func_088853CC(void *object, char value);
void func_088858E4(void *object, char value);
void func_08A1E188(void *object);
void func_08A20358(void *object);
}
struct Object9D69E8 {
    static void Hold(char value) { func_088853CC(D_089D69E8, value); }
    static void Release(char value) { func_088858E4(D_089D69E8, value); }
};
extern "C" void func_08A0C240(void)
{
    Object9D69E8::Hold(1);
    func_08A1E188(D_08A23FF0);
    func_08A20358(D_08A23FF0);
    Object9D69E8::Release(0);
}
