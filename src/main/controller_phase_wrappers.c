/* The registered setup/update/teardown wrappers all use the controller stored
 * at this main global. Full controller and handler types remain unresolved. */
extern void *D_089D6A10;
extern void func_0888C4C8(void *);
extern void func_0888DFA4(void *);
extern void func_08888F90(void *);

void func_08889098(void) { func_0888C4C8(D_089D6A10); }
void func_088890B8(void) { func_0888DFA4(D_089D6A10); }
void func_088890D8(void) { func_08888F90(D_089D6A10); }
