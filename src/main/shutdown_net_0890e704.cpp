// Shut down the network helper: free its buffer, stop the services and clear the running flag.
extern "C" unsigned char D_089D836C;
extern "C" int D_089D8368;
extern "C" int D_089D69A8;
extern "C" void func_08882298(int a, int b);
extern "C" void func_0890E850(void);
extern "C" void func_089C5A30(void);
extern "C" void func_089C58A8(int v);
extern "C" void func_0890E704(void)
{
    if (D_089D836C == 0) {
        return;
    }
    if (D_089D8368 != 0) {
        func_08882298(D_089D69A8, D_089D8368);
        D_089D8368 = 0;
    }
    func_0890E850();
    func_089C5A30();
    func_089C58A8(0x101);
    func_089C58A8(0x100);
    D_089D836C = 0;
}
