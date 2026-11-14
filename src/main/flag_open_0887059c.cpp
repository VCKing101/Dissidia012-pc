extern "C" {
void func_088F45FC(void *self);
int func_088F47E0(int id);
int func_0886F424(void *self, int mode);
}

struct GameFlags {
    static bool isSet(int id) { return func_088F47E0(id) != 0; }
};

extern "C" {

int func_0887059C(void *self)
{
    func_088F45FC(self);
    if (GameFlags::isSet(0x14)) {
        return func_0886F424(self, 1);
    } else {
        return 0;
    }
}

}
