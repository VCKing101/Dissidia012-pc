typedef struct Agent4Params {
    unsigned char unknown_0000[0x30];
    float field_30;
    float field_34;
} Agent4Params;

typedef struct Agent4ParamOwner {
    unsigned char unknown_0000[0x568];
    Agent4Params *params;
} Agent4ParamOwner;

typedef struct Agent4ParamUser {
    unsigned char unknown_0000[0x24];
    Agent4ParamOwner *owner;
    unsigned char unknown_0028[9];
    signed char level;
} Agent4ParamUser;

float func_08823A64(Agent4ParamUser *self) {
    return self->owner->params->field_30;
}

float func_08823A74(Agent4ParamUser *self) {
    float value = self->owner->params->field_34;
    if (self->level > 0 && self->level < 5) {
        return 0.0f;
    }
    return value;
}
