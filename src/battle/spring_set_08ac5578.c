typedef struct Spring {
    float rate;
    float velocity;
} Spring;

void func_08AC5578(Spring *self, float rate)
{
    self->rate = rate;
    self->velocity = 0.0f;
}
