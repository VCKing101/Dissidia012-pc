/* Registers a listener in the first free of the four global slots / removes it again; 1 on success. */
extern "C" {
extern int D_09B69868[4];
int func_088CB00C(int value)
{
    for (int i = 0; i < 4; i++) {
        if (D_09B69868[i] != 0) {
            continue;
        }
        D_09B69868[i] = value;
        return 1;
    }
    return 0;
}
int func_088CB054(int value)
{
    for (int i = 0; i < 4; i++) {
        if (D_09B69868[i] != value) {
            continue;
        }
        D_09B69868[i] = 0;
        return 1;
    }
    return 0;
}
}
