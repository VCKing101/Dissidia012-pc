/* Inverse of a random rate drawn from the second range (1 when not positive). */
typedef struct Range08820B9C {
    float min;
    float max;
} Range08820B9C;

typedef struct Ranges08820B9C {
    int field_00;
    Range08820B9C first;
    int field_0C;
    int field_10;
    Range08820B9C second;
} Ranges08820B9C;

typedef struct Owner08820B9C {
    int field_00;
    Ranges08820B9C *ranges;
} Owner08820B9C;

float func_08820CBC(Owner08820B9C *self, float lo, float hi);

float func_08820B9C(Owner08820B9C *self)
{
    float rate = func_08820CBC(self, self->ranges->second.min, self->ranges->second.max);
    if (rate <= 0.0f) {
        rate = 1.0f;
    }
    return 1.0f / rate;
}
