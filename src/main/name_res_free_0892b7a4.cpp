extern "C" {
typedef struct NameRes { char pad_00[0x24]; int models[4]; } NameRes;
extern void func_0887BA5C(int model);
void func_0892B7A4(NameRes *res)
{
    for (int i = 0; i < 4; i++) {
        if (res->models[i] != 0) {
            func_0887BA5C(res->models[i]);
            res->models[i] = 0;
        }
    }
}
}
