/* Clears two words and a byte at the start of the struct. */
typedef struct C3 { int a; int b; char c; } C3;
void func_08B45288(C3 *p) { p->a = 0; p->b = 0; p->c = 0; }
