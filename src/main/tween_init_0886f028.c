typedef struct Tween { int id; float t; float speed; int field_0C; int flags; } Tween;
Tween *func_0886F028(Tween *self)
{
    self->id = -1;
    self->t = 0.0f;
    self->speed = 0.0f;
    self->flags = 0;
    return self;
}
