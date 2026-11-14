/* Clears a fade record. */
typedef struct FadeRecord {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
    int field_10;
    float time;
    float rate;
    unsigned char active;
    unsigned char done;
} FadeRecord;

void func_088E715C(FadeRecord *fade)
{
    fade->field_00 = 0;
    fade->field_04 = 0;
    fade->field_08 = 0;
    fade->field_0C = 0;
    fade->field_10 = 0;
    fade->time = 0.0f;
    fade->rate = 0.0f;
    fade->active = 0;
    fade->done = 0;
}
