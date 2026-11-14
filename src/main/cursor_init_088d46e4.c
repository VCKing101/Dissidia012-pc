/* Initialises a cursor (type 8) with its range and step and rewinds it. */
typedef struct Cursor088D46E4 {
    int type;
    int a;
    int b;
    int c;
    int d;
    int d2;
    int e;
    int f1c;
    int f20;
    int f24;
    int f28;
} Cursor088D46E4;

void func_088D4744(Cursor088D46E4 *cur);
void func_088D46E4(Cursor088D46E4 *cur, int a, int b, int c, int d, int e)
{
    int zero = 0;
    cur->type = 8;
    cur->a = a;
    cur->b = b;
    cur->c = c >= 0 ? c : zero;
    cur->d = d;
    cur->d2 = d;
    cur->e = e;
    cur->f1c = 0;
    cur->f28 = 0;
    func_088D4744(cur);
}
