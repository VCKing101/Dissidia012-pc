/* 1 when the side's fighter has body type 0x17, 0 for any other body, -1
 * when the side or its body data is missing. */
typedef struct BodyFighter {
    unsigned char pad_000[0x5A4];
    unsigned char *body;
} BodyFighter;

extern BodyFighter *func_089E95B0(unsigned int side);

int func_089E98F0(unsigned int side)
{
    BodyFighter *fighter;

    if (side >= 2) {
        return -1;
    }
    fighter = func_089E95B0(side);
    if (fighter == 0) {
        return -1;
    }
    if (fighter->body == 0) {
        return -1;
    }
    if (*fighter->body != 0x17) {
        return 0;
    }
    return 1;
}
