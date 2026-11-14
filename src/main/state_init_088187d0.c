/* Resets a playback state block. */
typedef struct State088187D0 {
    char pad_00[0xC];
    unsigned char active;
    char pad_0D[3];
    int mode;
    int field_14;
    char pad_18[0xC];
    unsigned char field_24;
    char pad_25[3];
    float volume;
    int field_2C;
    unsigned char field_30;
    char pad_31[3];
    float field_34;
    int field_38;
    float field_3C;
    int field_40;
    unsigned char field_44;
    unsigned char field_45;
    char pad_46[2];
    int field_48;
    int field_4C;
} State088187D0;

void func_088187D0(State088187D0 *state)
{
    state->active = 0;
    state->mode = 0;
    state->field_14 = 0;
    state->field_24 = 0;
    state->volume = 0.0f;
    state->field_2C = 0;
    state->field_30 = 0;
    state->field_34 = 0.0f;
    state->field_38 = 0;
    state->field_3C = 0.0f;
    state->field_40 = -1;
    state->field_44 = 0;
    state->field_45 = 0;
    state->field_48 = 0;
    state->field_4C = 0;
}
