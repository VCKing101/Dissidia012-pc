/* Stores the 4-float result of the lookup (kind 2 below four, kind 6 otherwise) into out. */
struct V4 { float x; float y; float z; float w; };
extern "C" {
V4 func_08A803AC(int kind, int arg);
void func_08A8B538(V4 *out, int unused, int arg, int level)
{
    if (level < 4) {
        *out = func_08A803AC(2, arg);
    } else {
        *out = func_08A803AC(6, arg);
    }
}
}
