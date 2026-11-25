/* Loads "original_quest_param.bin": owner into field 1; file = func_08862C40(name, 1); field 0 = func_0885C6CC(owner, file, 0, 0, 1, 0); func_08862854(name, h, file, 1); field 4 = field 0; func_0885C93C(); field 3 = word 8 of func_0885CA3C(field 4, 1, 0). */
extern char D_089CEAA4[];
extern void *func_08862C40(char *name, int a);
extern int func_0885C6CC(int owner, void *file, int a, int b, unsigned char c, int d);
extern void func_08862854(char *name, int h, void *file, int a);
extern void func_0885C93C(void);
extern int *func_0885CA3C(int h, int a, int b);
void func_08927528(int *self, int owner)
{
    void *file;
    int h;
    int *r;
    self[1] = owner;
    file = func_08862C40(D_089CEAA4, 1);
    int one = 1;
    h = func_0885C6CC(self[1], file, 0, 0, one, 0);
    self[0] = h;
    func_08862854(D_089CEAA4, h, file, 1);
    self[4] = self[0];
    func_0885C93C();
    r = func_0885CA3C(self[4], 1, 0);
    self[3] = r[2];
}
