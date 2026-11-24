// Prepares the base, then starts the entry for the current mode with the stored pair and five forwarded values.
struct S { char d[0x28]; int ready; int h2c; int h30; int mode; char e[4]; char ent[1]; };
extern void func_0892CF8C(struct S *s);
extern void func_0892C868(char *ent, int a, int b, int k, int p1, int p2, int p3, int p4, int p5);
void func_0892CD98(struct S *s, int p1, int p2, int p3, int p4, int p5)
{
    unsigned short loc[2];
    int m;
    func_0892CF8C(s);
    int c1 = 1;
    int c3 = 3;
    loc[0] = c1;
    m = s->mode;
    loc[1] = c3;
    func_0892C868((char *)s + m * 0x28 + 0x38, s->h2c, s->h30, loc[m], p1, p2, p3, p4, p5);
    s->ready = 1;
}
