/* Starts a battle in FIELD mode 12: sets a flag in the object at *(0x089D7E88)+0x184D, registers callback 0x08A50160 in slots 0 to 2 (main 0x08889ED4), sets mode 12 sub-mode 2 (0x0888964C) and begins the battle (0x08889808). */
extern "C" char *D_089D7E88;
extern "C" void *D_089D6A10;
extern "C" void func_08A50160(void);
extern "C" void func_08889ED4(int slot, void (*callback)(void), int on);
extern "C" void func_0888964C(void *field, int mode, int a, int b, int c);
extern "C" void func_08889808(void *field);
extern "C" void func_08A505C0(void)
{
    void *f = D_089D6A10;
    D_089D7E88[0x184D] = 1;
    
    func_08889ED4(0, func_08A50160, 1);
    
    func_08889ED4(1, func_08A50160, 1);
    
    func_08889ED4(2, func_08A50160, 1);
    func_0888964C(D_089D6A10, 12, 0, 2, 0);
    func_08889808(D_089D6A10);
}
