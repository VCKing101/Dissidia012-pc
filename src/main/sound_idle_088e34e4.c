/* True when the sound manager reports nothing playing. */
extern void *D_089D6998;
extern int func_0887E718(void *manager);

int func_088E34E4(void)
{
    if (func_0887E718(D_089D6998) != 0) {
        return 0;
    }
    return 1;
}
