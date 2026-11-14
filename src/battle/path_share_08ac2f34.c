typedef struct PathSegment {
    char pad_00[0x8];
    float start;
    float end;
} PathSegment;

typedef struct PathCursor {
    char pad_00[0x18];
    int index;
    int path;
} PathCursor;

extern float D_08AC3C10;
extern float func_08AC303C(PathCursor *self);
extern PathSegment *func_08ABC420(int path, int index);
extern float func_08AC3D20(float start, float end);

/* Share of the path's length taken by the current segment; the default when there is none. */
float func_08AC2F34(PathCursor *self)
{
    float total = func_08AC303C(self);
    PathSegment *segment;
    if (total <= 0.0f) {
        return D_08AC3C10;
    }
    segment = func_08ABC420(self->path, self->index);
    if (segment == 0) {
        return D_08AC3C10;
    }
    return func_08AC3D20(segment->start, segment->end) / total;
}
