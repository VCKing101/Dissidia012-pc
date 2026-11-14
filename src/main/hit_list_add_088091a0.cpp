extern "C" {
struct Vec3 {
    float x;
    float y;
    float z;
};
extern void func_089A34C0(Vec3 *dst, const Vec3 *src);
struct Hit {
    int id;
    int kind;
    float power;
    float range;
    int flags;
    Vec3 pos;
    Hit &operator=(const Hit &other)
    {
        id = other.id;
        kind = other.kind;
        power = other.power;
        range = other.range;
        flags = other.flags;
        func_089A34C0(&pos, &other.pos);
        return *this;
    }
};
struct HitList {
    int count;
    Hit hits[250];
};
bool func_088091A0(HitList *list, Hit *hit)
{
    if (list->count < 250) {
        list->hits[list->count++] = *hit;
        return true;
    }
    return false;
}
}
