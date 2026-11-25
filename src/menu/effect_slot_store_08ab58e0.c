/* Stores a halfword into the slot of an effect record chosen by index (0 to 15). */
typedef struct R { char p[0x4C]; short h4C; short h4E; short h50; short h52; short h54; short h56; short h58; short h5A; short h5C; short h5E; short h60; short h62; short h64; short h66; short h68; short h6A; } R;
void func_08AB58E0(R *r, int idx, int v)
{
    int k;
    if (idx == 0) {
        r->h4C = v;
    } else if (idx == (k = 1)) {
        r->h4E = v;
    } else if (idx == (k = 2)) {
        r->h50 = v;
    } else if (idx == (k = 3)) {
        r->h52 = v;
    } else if (idx == (k = 4)) {
        r->h54 = v;
    } else if (idx == (k = 5)) {
        r->h56 = v;
    } else if (idx == (k = 6)) {
        r->h5A = v;
    } else if (idx == (k = 7)) {
        r->h60 = v;
    } else if (idx == (k = 8)) {
        r->h5C = v;
    } else if (idx == (k = 9)) {
        r->h5E = v;
    } else if (idx == (k = 10)) {
        r->h58 = v;
    } else if (idx == (k = 11)) {
        r->h62 = v;
    } else if (idx == (k = 12)) {
        r->h64 = v;
    } else if (idx == (k = 13)) {
        r->h66 = v;
    } else if (idx == (k = 14)) {
        r->h68 = v;
    } else if (idx == (k = 15)) {
        r->h6A = v;
    }
}
