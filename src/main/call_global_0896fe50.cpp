// Calls func_089A04F4 with 0 and the word at D_089DA280 (read through a volatile access so the return-address save is scheduled first).
extern "C" int D_089DA280;
extern "C" void func_089A04F4(int, int);
extern "C" void func_0896FE50(void) {
 func_089A04F4(0, *(volatile int *)&D_089DA280);
}
