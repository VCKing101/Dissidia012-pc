/* Resets the 35 0x30-byte vector groups. */
struct Group088B312C {
    char data[0x30];
};

extern "C" {

void func_088B30CC(Group088B312C *group);

void func_088B312C(Group088B312C *groups)
{
    for (int i = 0; i < 35; i++) {
        func_088B30CC(&groups[i]);
    }
}

}
