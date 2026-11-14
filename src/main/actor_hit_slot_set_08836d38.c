typedef struct HitSlot { int x; int y; float value; float time; unsigned char used; unsigned char tag; char pad_12[2]; } HitSlot;
typedef struct Actor08836D38 { char pad_000[0x4AC]; HitSlot *slots; } Actor08836D38;
void func_08836D38(Actor08836D38 *a, int index, int x, int y, int value, unsigned char tag)
{
    HitSlot *s = &a->slots[index];
    if (!s->used) {
        s->x = x;
        s->y = y;
        s->value = value;
        s->time = 0.0f;
        s->used = 1;
        s->tag = tag;
    }
}
