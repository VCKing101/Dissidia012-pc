/* Resets the 31 0x57C-byte groups. */
struct Group088C0450 {
    char data[0x57C];
};

extern "C" {

void func_088C0228(Group088C0450 *group);

void func_088C0450(Group088C0450 *groups)
{
    for (int i = 0; i < 31; i++) {
        func_088C0228(&groups[i]);
    }
}

}
