extern int func_089C57E0(int module, int argsize, void *argp, int *status, void *option); /* stop module */
extern int func_089C57C0(int module);                                                     /* unload module */

void func_0886EF94(void *self, int module)
{
    int kept[1];

    kept[0] = module;
    int r = func_089C57E0(module, 0, 0, 0, 0);
    module = kept[0];
    if (r == 0) {
        func_089C57C0(module);
    }
}
