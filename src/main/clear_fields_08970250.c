// Zeroes two words, a byte and a halfword at the start of the object.
struct S08970250 { int a, b; char c; char pad; short d; };
void func_08970250(struct S08970250 *p) { p->a = 0; p->b = 0; p->c = 0; p->d = 0; }
