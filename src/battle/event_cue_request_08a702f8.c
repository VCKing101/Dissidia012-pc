/* Handles a kind-3 cue request: clears the cue for a grounded fighter when
 * asked to, or raises the requested cue once the fighter's resource is
 * ready (-1 bad arguments, -2 not a kind-3 request, -3 no fighter). */
typedef struct CueRequest {
    unsigned char pad_00[8];
    unsigned int kind;
    int cue;
    int mode;
    int clear;
} CueRequest;

typedef struct RequestInfo {
    unsigned char pad_00[4];
    void *fighter;
} RequestInfo;

typedef struct RequestEvent {
    unsigned char pad_00[0x64];
    void *owner;
    void *cues;
} RequestEvent;

extern RequestInfo *func_08AB3264(void *owner);
extern int func_088374B0(void *fighter);
extern int func_08AB344C(void *fighter);
extern void func_08A969BC(void *cues, int cue);
extern void func_08A96BAC(void *cues, int cue);

int func_08A702F8(RequestEvent *self, CueRequest *request, void *context, void *flag)
{
    void *fighter;
    if (request == 0 || context == 0 || flag == 0) {
        return -1;
    }
    if (request->kind != 3 || request->kind >= 6 || request->mode != 3) {
        return -2;
    }
    fighter = func_08AB3264(self->owner)->fighter;
    if (fighter == 0) {
        return -3;
    }
    if (func_088374B0(fighter) == 0 && request->clear == 1) {
        func_08A969BC(self->cues, 0);
    } else if (func_08AB344C(fighter) == 1) {
        func_08A96BAC(self->cues, request->cue);
    }
    return 0;
}
