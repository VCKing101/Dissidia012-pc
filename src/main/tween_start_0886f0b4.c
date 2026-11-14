typedef struct Tween { int id; float t; float speed; float target; unsigned int flags; } Tween;
void func_0886F0B4(Tween *self, int id, float speed, float target)
{
    self->id = id;
    if (speed == 0.0f) {
        self->t = 0.0f;
    }
    self->target = target;
    self->speed = speed;
    self->flags |= 1;
}
