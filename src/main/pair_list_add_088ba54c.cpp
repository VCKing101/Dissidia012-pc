/* Adds a count to a (key, value) pair list, appending the key when it is new and its record is open. */
struct Rec { short a; short v; unsigned short fl; bool closed() { return (fl & 2) != 0; } };
extern "C" int func_088BA66C(int, int, short *);
extern "C" Rec *func_088B8414(int, int, int);
extern "C" int func_088BA54C(short *out, short *ids, int n, short key, int add, int tbl)
{
    if (key != -1) {
        short *end = ids + n;
        short *hit = (short *)func_088BA66C((int)ids, (int)end, &key);
        if (hit == end) {
            int one = 1; Rec *rec = func_088B8414(tbl, key, one);
            if (rec != 0 && !rec->closed()) {
                short k = key; short *o = out + n * 2;
                o[0] = k;
                o[1] = rec->v + add;
                ids[n] = k;
                n = n + 1;
            }
        } else {
            short *o = out + (hit - ids) * 2;
            o[1] = o[1] + add;
        }
    }
    return n;
}
