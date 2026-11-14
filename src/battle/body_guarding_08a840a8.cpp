typedef struct BodyStatus {
    char pad_0[0x3];
    unsigned char flags;

    static unsigned char first(BodyStatus *status)
    {
        unsigned int bit = ((unsigned char *)status)[3] & 1;
        return bit;
    }
    static unsigned char second(BodyStatus *status)
    {
        unsigned int bit = (((unsigned char *)status)[3] & 2) != 0;
        return bit;
    }
    static unsigned char third(BodyStatus *status)
    {
        unsigned int bit = (((unsigned char *)status)[3] & 4) != 0;
        return bit;
    }
} BodyStatus;

typedef struct GuardBody {
    char pad_000[0x3C7];
    signed char stance;
    char pad_3C8[0x1E8];
    BodyStatus status;
} GuardBody;

struct GuardOps {
    static unsigned char guarding(GuardBody *body)
    {
        int result = 0;
        if (BodyStatus::first(&body->status) == 1 || BodyStatus::second(&body->status) == 1 ||
            BodyStatus::third(&body->status) == 1) {
            result = 1;
        }
        return result;
    }
};

extern "C" {

unsigned char func_08A840A8(GuardBody *body)
{
    if (body == 0) {
        return 0;
    }
    if (body->stance != 5) {
        return 0;
    }
    return GuardOps::guarding(body);
}

}
