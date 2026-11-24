/* Looks a name up in the twenty-entry name table and returns its value, or
   -1 when it is not listed. */
extern "C" {
struct NameEntry {
    const char *name;
    int value;
};

extern NameEntry D_089CAA10[];

int func_0893585C(const char *a, const char *b);

int func_088967E0(const char *name)
{
    for (unsigned int i = 0; i < 20; i++) {
        if (func_0893585C(name, D_089CAA10[i].name) == 0) {
            return D_089CAA10[i].value;
        }
    }
    return -1;
}

}
