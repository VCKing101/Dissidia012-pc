// When the mode byte at +0x44 is 1, queries the pair and stores the two values at +0x34/+0x38 in the order the query result picks.
struct T { int x; int y; char pad[0x2C]; int a; int b; char pad2[8]; unsigned char mode; };
extern void func_089C5AE0(int *, char *, char *, int);
void func_08973650(struct T *t) {
    int v;
    char q1[4];
    char q2[4];

    if (t->mode == 1) {
        func_089C5AE0(&v, q1, q2, 0);
        int answer = v; int tmp = t->b;
        if (answer == tmp) {
            t->x = tmp;
            t->y = t->a;
        } else {
            t->x = t->a;
            t->y = tmp;
        }
    }
}
