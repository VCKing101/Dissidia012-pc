typedef struct System08850AB8 {
    int active;
} System08850AB8;

extern System08850AB8 *D_089D4DA8;
extern unsigned char D_09B16930[];

void func_08935524(void *pool, int a, int b);
void func_08850888(System08850AB8 *system, void *pool);

void func_08850AB8(int a, int b) {
    if (D_089D4DA8->active != 0) {
        return;
    }
    func_08935524(D_09B16930, a, b);
    func_08850888(D_089D4DA8, D_09B16930);
}
