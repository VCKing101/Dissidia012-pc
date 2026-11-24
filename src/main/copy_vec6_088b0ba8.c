/* Copy six words into the global record and notify the owner. */
typedef struct V6 { int v[6]; } V6;
typedef struct Own { char pad[0x14]; int rec; } Own;
extern Own *D_089D7260;
extern void func_088AFB08(Own *o);
void func_088B0BA8(V6 *src)
{
    V6 *p = (V6 *)(D_089D7260->rec + 0xEA48);
    *p = *src;
    func_088AFB08(D_089D7260);
}
