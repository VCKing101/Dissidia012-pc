typedef struct System08811A58 {
    int active;
} System08811A58;

extern System08811A58 *D_089D47A0;
extern unsigned char D_09AF8B20[];

void func_08935524(void *pool, int a, int b);
void func_08811A04(System08811A58 *system, void *pool);

void func_08811A58(int a, int b) {
    if (D_089D47A0->active != 0) {
        return;
    }
    func_08935524(D_09AF8B20, a, b);
    func_08811A04(D_089D47A0, D_09AF8B20);
}
