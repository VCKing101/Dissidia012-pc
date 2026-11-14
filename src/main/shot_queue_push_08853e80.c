typedef struct Shot08853E80 { int target; short kind; unsigned char f6; unsigned char style; unsigned char team; char pad_09[3]; float x; float y; float z; } Shot08853E80;
typedef struct ShotQueue08853E80 { Shot08853E80 shots[2]; int count; } ShotQueue08853E80;
int func_08853E80(ShotQueue08853E80 *q, int target, int kind, int style, int team, float x, float y, float z)
{
    int n = q->count;
    Shot08853E80 *s;
    if (n >= 2) {
        return 0;
    }
    s = &q->shots[n];
    s->target = target;
    s->kind = kind;
    s->x = x;
    s->style = style;
    s->f6 = 0;
    s->y = y;
    s->z = z;
    s->team = team;
    q->count = n + 1;
    return 1;
}
