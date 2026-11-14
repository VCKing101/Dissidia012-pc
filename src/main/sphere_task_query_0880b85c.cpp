#include "main/agent30_types.h"

/* Runs a sphere hit query for the task's area and hands the result to the
   given collector. */
struct Shape0880B85C {
    unsigned char a;
    unsigned char b;
    short c;
    unsigned char type;
    unsigned char kind;
    short d;
    float e[4];
    float x;
    float y;

    Shape0880B85C()
    {
        a = 0;
        b = 0;
        c = 0;
        d = 0;
        e[0] = 0.0f;
        e[1] = 0.0f;
        e[2] = 0.0f;
        e[3] = 0.0f;
    }
};

struct Link0880B85C {
    int kind;
    float x;
    float y;
    Shape0880B85C *source;
    float v[3];

    Link0880B85C() { source = 0; }
};

struct Query0880B85C {
    void *owner;
    Link0880B85C link;
};

struct SphereTask0880B85C {
    char pad_00[0x80];
    Vec4 centre;
    char pad_90[0xA0 - 0x90];
    float radius;
};

extern "C" {

void func_0881276C(Link0880B85C *link, Vec4 *centre, Shape0880B85C *shape);
void func_088091A0(void *collector, Query0880B85C *query);

void func_0880B85C(SphereTask0880B85C *self, void *collector)
{
    Query0880B85C q;
    Shape0880B85C shape;
    shape.x = self->radius;
    shape.y = self->radius;
    shape.type = 4;
    shape.kind = 3;
    q.owner = self;
    func_0881276C(&q.link, &self->centre, &shape);
    func_088091A0(collector, &q);
}

}
