extern "C" {
typedef struct LinkSpan {
    short count;
    short start;
} LinkSpan;

typedef struct NodeGraph {
    char pad_00[0x6C];
    unsigned int count;
    LinkSpan *spans;
} NodeGraph;

typedef struct GraphOwner {
    unsigned char active;
    char pad_01[0x3];
    NodeGraph *graph;
} GraphOwner;

extern int func_08AA7BE8(GraphOwner *self, int link);

/* 1 when any link of node is blocked (or node is out of range), 0 otherwise. */
int func_08AA7C80(GraphOwner *self, unsigned int node)
{
    NodeGraph *graph;
    LinkSpan *span;
    if (self->active == 0) {
        return 0;
    }
    graph = self->graph;
    if (node >= graph->count) {
        return 1;
    }
    span = &graph->spans[node];
    for (int i = 0; i < span->count; i++) {
        if (func_08AA7BE8(self, span->start + i) != 0) {
            return 1;
        }
    }
    return 0;
}
}
