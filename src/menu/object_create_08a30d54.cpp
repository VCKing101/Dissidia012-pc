extern "C" {
extern int func_08A408E8(int size);
extern void func_08A33B48(int obj);
}

struct Inner30D54 { char pad[0x6B0]; int flag; };
struct Obj30D54 { char pad[0x6DC]; Inner30D54 *inner; };

struct Create30D54 {
    static Obj30D54 *Make() {
        Obj30D54 *result = 0;
        Obj30D54 *obj = (Obj30D54 *)func_08A408E8(0x6E0);
        if (obj != 0) {
            func_08A33B48((int)obj);
            result = obj;
        }
        return result;
    }
};

extern "C" Obj30D54 *func_08A30D54(unsigned char off)
{
    Obj30D54 *obj = Create30D54::Make();
    obj->inner->flag = (off == 0);
    return obj;
}
