/* Sets the cursor limits, rewinds it and advances it; 1 when its index changed. */
typedef struct Cursor088D4788 {
    int type;
    int index;
    int b;
    int c;
    int d;
    int d2;
    int e;
    int f1c;
    int f20;
    int f24;
    int f28;
} Cursor088D4788;

void func_088D4744(Cursor088D4788 *cur);
int func_088D35E0(Cursor088D4788 *cur, int index, int arg);
void func_088D47F4(Cursor088D4788 *cur);
int func_088D4788(Cursor088D4788 *cur, int f28, int f1c)
{
    int changed = 0;
    unsigned int old;
    unsigned int now;
    cur->f1c = f1c;
    cur->f28 = f28;
    func_088D4744(cur);
    old = cur->index;
    now = func_088D35E0(cur, old, 1);
    cur->index = now;
    if (old != now) {
        func_088D47F4(cur);
        changed = 1;
    }
    return changed;
}
