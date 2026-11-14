/* Float parameters drawn at random between a stored minimum and maximum. */

typedef struct Agent4RangeSetB {
    int field_00;
    int field_04;
    int field_08;
    float field_0C;
    float field_10;
    float field_14;
    float field_18;
    float field_1C;
    float field_20;
    float field_24;
    float field_28;
    float field_2C;
    float field_30;
    float field_34;
    float field_38;
    float field_3C;
    float field_40;
    float field_44;
} Agent4RangeSetB;

typedef struct Agent4RangeOwnerB {
    int field_00;
    Agent4RangeSetB *ranges;
} Agent4RangeOwnerB;

/* Random float between min and max (a method; self is unused). */
float func_08820CBC(Agent4RangeOwnerB *self, float min, float max);

float func_08820BE4(Agent4RangeOwnerB *self) {
    return func_08820CBC(self, self->ranges->field_0C, self->ranges->field_10);
}

float func_08820C08(Agent4RangeOwnerB *self) {
    return func_08820CBC(self, self->ranges->field_1C, self->ranges->field_20);
}

float func_08820C2C(Agent4RangeOwnerB *self) {
    return func_08820CBC(self, self->ranges->field_24, self->ranges->field_28);
}

float func_08820C50(Agent4RangeOwnerB *self) {
    return func_08820CBC(self, self->ranges->field_2C, self->ranges->field_30);
}

float func_08820C74(Agent4RangeOwnerB *self) {
    return func_08820CBC(self, self->ranges->field_38, self->ranges->field_3C);
}

float func_08820C98(Agent4RangeOwnerB *self) {
    return func_08820CBC(self, self->ranges->field_40, self->ranges->field_44);
}
