"""Check presentation channel geometry, bounded events, and source forwarding."""
import os
import subprocess
import unittest
from pathlib import Path
from compile_psp import compile_object

ROOT = Path(__file__).resolve().parents[1]


class PresentationServiceChecks(unittest.TestCase):
    def test_native_event_layout_and_channel_stride(self):
        directory = ROOT / "build/presentation-service-tests"
        directory.mkdir(parents=True, exist_ok=True)
        source = directory / "layout.c"
        source.write_text(r'''
#include "main/presentation_channel.h"
#define OFFSET(t,m) ((unsigned int)&(((t *)0)->m))
typedef char VectorSize[(sizeof(MainPresentationVector)==16)?1:-1];
typedef char EventSize[(sizeof(MainPresentationEvent)==0x20)?1:-1];
typedef char ChannelStride[(sizeof(MainPresentationChannel)==0x240)?1:-1];
typedef char EventsAt[(OFFSET(MainPresentationChannel,events_0020)==0x20)?1:-1];
typedef char CountAt[(OFFSET(MainPresentationChannel,field_0220)==0x220)?1:-1];
typedef char MarkerAt[(OFFSET(MainPresentationChannel,field_022C)==0x22C)?1:-1];
typedef char ControlAt[(OFFSET(MainPresentationChannel,field_0234)==0x234)?1:-1];
typedef char PayloadAt[(OFFSET(MainPresentationEvent,field_0014)==0x14)?1:-1];
''', encoding="utf-8")
        result = compile_object("tools/compilers/pspsnc_1.2.7503.0/pspsnc.exe", ["-O2", "-g"],
                                source.relative_to(ROOT), (directory / "layout.raw.o").relative_to(ROOT))
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)

    def test_source_preserves_capacity_markers_payloads_and_returns(self):
        directory = ROOT / "build/presentation-service-tests"
        directory.mkdir(parents=True, exist_ok=True)
        harness = directory / "harness.c"
        harness.write_text(r'''
#include "main/presentation_channel.h"
#include "main/drawing_record.h"
#include <assert.h>
#include <math.h>
#include <string.h>
MainPresentationChannel D_09B47B50[4];
const MainPresentationVector D_09B48210[4] = {{{1,2,3,4}},{{5,6,7,8}},{{9,10,11,12}},{{13,14,15,16}}};
static int copies, transition_calls, acquired, activated, callback_argument;
static void completed(int argument) { callback_argument = argument; }
static MainPresentationChannel *transition_seen;
static unsigned int kind_seen;
static int second_seen;
static MainPresentationCallback callback_seen;
static float time_seen;
static MainDrawingListPrefix drawing_owner;
static MainDrawingRecordPrefix drawing_record;
MainPresentationVector *func_089A3868(MainPresentationVector *out,const float *in) {
    MainPresentationVector copy;
    memcpy(copy.values,in,sizeof(copy.values)); *out = copy; copies++; return out;
}

void func_08874350(MainPresentationChannel *channel,unsigned int kind,MainPresentationCallback callback,int second,float time) {
    transition_calls++; transition_seen = channel; kind_seen = kind;
    callback_seen = callback; second_seen = second; time_seen = time;
}
void *func_088DAA40(void *owner,int first,unsigned char second) {
    assert(owner == &drawing_owner && first == -1 && second == 0);
    acquired++; return &drawing_record;
}
void func_088D9CFC(MainDrawingRecordPrefix *record) {
    assert(record == &drawing_record && acquired == 1); activated++;
}
int main(void) {
    MainPresentationChannel *channel = &D_09B47B50[1];
    float input[4] = {21,22,23,24};
    assert(func_08873B18(&channel->value,input) == &channel->value);
    assert(!memcmp(channel->value.values,input,sizeof(input)));
    assert(func_08873B34(&channel->value,3) == &channel->value);
    assert(!memcmp(channel->value.values,D_09B48210[3].values,sizeof(input)));
    assert(func_08873DB0(input,2) == &D_09B47B50[2].value);
    assert(!memcmp(D_09B47B50[2].value.values,input,sizeof(input)));
    channel->field_0234 = 17; func_08873B5C(channel); assert(channel->field_0234 == 0);
    channel->field_022C = 0;
    int before = copies;
    assert(!func_08873B64(channel,input,completed,-7,0));
    assert(!func_08873B64(channel,input,completed,-7,-1));
    assert(channel->field_0220 == 0 && copies == before);
    assert(func_08873C20(channel,2,completed,-7,1) == 1);
    assert(channel->field_0220 == 1 && channel->field_022C == 1);
    assert(!memcmp(channel->events_0020[0].value.values,D_09B48210[2].values,sizeof(input)));
    assert(channel->events_0020[0].field_0014 == completed && channel->events_0020[0].field_0018 == -7);
    channel->events_0020[0].field_0014(channel->events_0020[0].field_0018);
    assert(callback_argument == -7);
    assert(channel->events_0020[0].field_0010 == 1);
    assert(!func_08873B64(channel,input,0,0,1));
    for (int i = 1; i < 16; i++) assert(func_08873B64(channel,input,0,i,(float)(i+1)));
    before = copies;
    assert(!func_08873B64(channel,input,0,0,100));
    assert(channel->field_0220 == 16 && channel->field_022C == 16 && copies == before);
    assert(channel->events_0020[15].field_0018 == 15);
    channel->field_0220 = 0; channel->field_022C = 0;
    assert(func_08873B64(channel,input,0,0,NAN));
    assert(isnan(channel->field_022C));
    assert(func_08873B64(channel,input,0,0,1)); /* Native <= comparisons are false for NaN. */
    assert(func_08873CE0(-123,0xF1,3) == &D_09B47B50[3]);
    assert(D_09B47B50[3].field_0230 == -123 && D_09B47B50[3].field_0238 == 0xF1);
    assert(D_09B47B50[3].field_0220 == 0 && D_09B47B50[3].field_0224 == 0);
    assert(D_09B47B50[3].field_0228 == -1 && D_09B47B50[3].field_022C == -1);
    assert(D_09B47B50[3].field_0234 == 1 && D_09B47B50[3].field_0239 == 0);
    assert(func_08873D34(4,completed,-17,(unsigned char)0x123,2,3.5f) == &D_09B47B50[2]);
    assert(D_09B47B50[2].field_0230 == 0 && D_09B47B50[2].field_0238 == 0x23);
    assert(transition_calls == 1 && transition_seen == &D_09B47B50[2] && kind_seen == 4);
    assert(callback_seen == completed && second_seen == -17 && time_seen == 3.5f);
    assert(func_088DAA68(&drawing_owner) == &drawing_record && acquired == 1 && activated == 1);
    assert(func_088D4C5C() == 15);
    return 0;
}
''', encoding="utf-8")
        relative = directory.relative_to(ROOT).as_posix()
        sources = ["presentation_events.c", "presentation_channels.c", "presentation_current.c",
                   "drawing_acquire_default.c", "presentation_default_input.c", "presentation_initialize.c"]
        commands = [["gcc", "-std=c99", "-O2", "-Wall", "-Wextra", "-Werror", "-Iinclude",
                     *("src/main/" + source for source in sources), relative + "/harness.c", "-o", relative + "/harness"],
                    ["./" + relative + "/harness"]]
        for command in commands:
            if os.name == "nt":
                linux_root = "/mnt/" + ROOT.drive[0].lower() + ROOT.as_posix()[2:]
                command = ["wsl", "-d", "Ubuntu-24.04", "--cd", linux_root, "--", *command]
            result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, timeout=30)
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__": unittest.main()
