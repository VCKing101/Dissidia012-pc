/* Runs and clears every queued callback. */
struct QueuedCallback {
    void (*fn)(int arg);
    int arg;
};

extern "C" {

extern int D_089D4E18;
extern QueuedCallback D_09B17D28[];

void func_0885C348(void)
{
    for (int i = 0; i < D_089D4E18; i++) {
        D_09B17D28[i].fn(D_09B17D28[i].arg);
        D_09B17D28[i].fn = 0;
        D_09B17D28[i].arg = 0;
    }
    D_089D4E18 = 0;
}

}
