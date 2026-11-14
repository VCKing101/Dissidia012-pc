/* The reach and height ranges of an attack, for the attack kinds that
   have them; 0 for every other kind. */
struct AttackDef0881334C {
    char pad_00[0x18];
    float min_reach;
    float reach;
    float height;
};

struct Attack0881334C {
    char pad_00[0x30];
    AttackDef0881334C *def;
    char pad_34;
    unsigned char kind;
};

extern "C" {

/* Reach and height for the melee kinds. */
void func_0881334C(Attack0881334C *self, float *reach, float *height)
{
    *reach = 0.0f;
    *height = 0.0f;
    switch (self->kind) {
    case 1:
    case 4:
    case 16:
    case 17:
    case 19:
        if (self->def != 0) {
            *reach = self->def->reach;
            *height = self->def->height;
        }
        break;
    default:
        break;
    }
}

/* Minimum reach and height for the ranged kinds. */
void func_088133B4(Attack0881334C *self, float *reach, float *height)
{
    *reach = 0.0f;
    *height = 0.0f;
    switch (self->kind) {
    case 3:
    case 5:
    case 6:
    case 7:
    case 8:
    case 20:
        if (self->def != 0) {
            *reach = self->def->min_reach;
            *height = self->def->height;
        }
        break;
    default:
        break;
    }
}

}
