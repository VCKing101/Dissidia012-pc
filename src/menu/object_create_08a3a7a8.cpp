extern "C" {
extern int func_08A408E8(int size);
extern void func_08A3A518(int obj);
extern int func_088FEA24(void);
}

struct Obj3A7A8 { char pad[0x1278]; int mode; unsigned char flag; };

struct Create3A7A8 {
    static Obj3A7A8 *Make() {
        Obj3A7A8 *result = 0;
        Obj3A7A8 *obj = (Obj3A7A8 *)func_08A408E8(0x1280);
        if (obj != 0) {
            func_08A3A518((int)obj);
            result = obj;
        }
        return result;
    }
};

extern "C" Obj3A7A8 *func_08A3A7A8(int mode)
{
    Obj3A7A8 *obj = Create3A7A8::Make();
    obj->mode = mode;
    obj->flag = 0;
    if (mode == 1) {
        if (func_088FEA24() != 0) {
            obj->flag = 1;
        }
    }
    return obj;
}
