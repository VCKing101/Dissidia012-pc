extern "C" {
extern int func_08A24090(int size);
extern void func_08A258F4(int obj, int arg);
}
struct O25A38 { static int Make(int arg) {
    int result = 0;
    int obj = func_08A24090(0x2900);
    if (obj != 0) {
        func_08A258F4(obj, arg);
        result = obj;
    }
    return result; } };
extern "C" int func_08A25A38(int arg) { return O25A38::Make(arg); }
