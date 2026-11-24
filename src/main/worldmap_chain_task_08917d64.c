/* Registers the WORLDMAP_CHAIN::MainTask task (func_08917DA8) on the work area at D_089D83DC + 0x100. */
extern char *D_089D83DC;
extern char D_089CE418[];
extern void func_08917DA8(void);
extern void func_08865088(void *work, int a, int b, void (*fn)(void), char *name);
void func_08917D64(void)
{
    func_08865088(D_089D83DC + 0x100, -1, 1000, func_08917DA8, D_089CE418);
}
