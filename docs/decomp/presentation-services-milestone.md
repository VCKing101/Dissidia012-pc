# Shared presentation services recovered

12 October 2026. **Ten additional functions / 636 source-built code bytes**
now match, bringing recovery to **244 C/C++ functions / 18,152 bytes**.
All four scoped files remain byte-identical to their pinned references;
**161 checks pass**. Emulator and data-install validation remain deferred.

| Module | Before: functions / bytes | Added: functions / bytes | After: functions / bytes |
| --- | ---: | ---: | ---: |
| Main | 74 / 4,652 | 10 / 636 | 84 / 5,288 |
| Extra | 24 / 2,116 | 0 / 0 | 24 / 2,116 |
| Menu | 44 / 4,320 | 0 / 0 | 44 / 4,320 |
| Battle | 92 / 6,428 | 0 / 0 | 92 / 6,428 |
| Total | **234 / 17,516** | **10 / 636** | **244 / 18,152** |

The [machine-readable record](presentation-services-milestone.json) lists every
addition, source path and checked native caller across the four modules.
Inventory revision **17** changes attribution only: the **21,280 candidate
entries / 4,717,908 function-body bytes** remain unchanged. All new credit
belongs to shared main helpers, without claiming complete gameplay ownership.

## Connected presentation services

| Address | Bytes | Observed behavior |
| --- | ---: | --- |
| `0x08873B18` | 28 | Forward a four-float copy and its returned pointer |
| `0x08873B34` | 40 | Select a vector from the indexed table and forward its copy |
| `0x08873B5C` | 8 | Clear the channel word at `0x234` |
| `0x08873B64` | 188 | Append a bounded pending record with a float marker and two payload words |
| `0x08873C20` | 40 | Append an indexed vector using the same insertion service |
| `0x08873CE0` | 84 | Select a channel, call its initializer and return the channel |
| `0x08873D34` | 124 | Initialize the selected channel, forward transition inputs and return the channel |
| `0x08873DB0` | 60 | Copy a vector into an indexed channel and preserve the returned pointer |
| `0x088DAA68` | 56 | Acquire a default drawing record, activate it and return it |
| `0x088D4C5C` | 8 | Return the fixed input value 15 used by menu callers |

The recovered source is grouped into contiguous
[event services](../../src/main/presentation_events.c),
[channel services](../../src/main/presentation_channels.c),
[current-vector forwarding](../../src/main/presentation_current.c),
[drawing acquisition](../../src/main/drawing_acquire_default.c) and the
[fixed input](../../src/main/presentation_default_input.c).

Native channel indexing establishes a **`0x240` stride**. Sixteen **`0x20`-byte
pending records** start at `0x20`, followed by the count at `0x220` and float
marker at `0x22C`. The
[shared layout view](../../include/main/presentation_channel.h) models the
four-float values, these observed fields and opaque gaps. The vector uses a
real four-element float array, allowing the existing menu caller's float-array
inputs to retain their types. Original class names, channel count and full
presentation ownership remain unknown.

Insertion rejects a full sixteen-record queue. For ordinary finite inputs,
it also rejects markers less than or equal to the previous marker. Accepted
records copy all four values and both payload words, update the count and
store the new marker. The native `<=` comparison is false for NaN, so the source
preserves that behavior. Channel and table indices retain the original valid
index preconditions; these wrappers introduce no new bounds checks.

The existing menu presentation callback now includes the same declarations
as the recovered services. Its previous local declarations omitted the pointer
returns. The corrected declarations preserve its exact **300-byte** source
match; it receives no additional credit for this interface correction. Menu
build fingerprints now include the shared header.

## Verification

[Presentation checks](../../tools/test_presentation_services.py) compile the real
PSP layout assertions and the recovered source with host service stubs. They
verify channel/event geometry, indexed copies, forwarded returns, payload
identity, byte flags, negative payload words, all sixteen event slots, rejected
equal/decreasing markers and NaN comparison behavior. Drawing acquisition is
checked for its default arguments, activation call and returned pointer.

The channel initializer, transition implementation and library vector-copy
service are stubs in those host checks. They remain separately uncredited;
the tests establish the recovered callers' contracts rather than completing
those dependencies. Raw object relocations, linked addresses and the complete
files are independently verified under unchanged SNC `-O2 -g` settings.

The main and menu files were rebuilt after integration. Battle and extra
retain their current verified proofs. All four proofs were validated together,
and the complete suite passes **161 tests**. No new runtime claim is made.

## Remaining source work

Task factory, registration and unlink still differ from the original. The
additional standard placement-construction, nullable-reference and member-method
probes are retained without credit. The optional-resource wait at
`0x089DDF24` also remains nonmatching; a constructed handle-reference object
reproduces its stack storage, but its control flow still differs.

The larger menu presentation setup at `0x089DE1D0` remains nonmatching.
Typed probe views reproduce its vector-copy pattern but differ in instruction
ordering. Its shared services now have exact source, giving the next attempt
better interface evidence. The five-case transition routine at `0x08874350`
has a 436-byte candidate against 432 original bytes and remains uncredited.
The four-float library copy at `0x089A3868` also remains uncredited; its floating
register allocation differs from the C candidate.

Next, continue presentation initialization/transition processing and the
larger controller setup. Revisit the task queue and optional-resource wait
with new compiler evidence rather than repeating the recorded variants.
The controller remains **14/20 handlers / 2,688 of 8,188 bytes**, and its
reviewed four-handler savedata slice remains fully source-matched. Save
serialization and whole-game completion remain unestablished. Main
`0x088D99B0` and menu `0x089DEA74` remain explicitly untraced.

No emulator was launched, no ISO or runtime package was rebuilt, and no saves
were changed.
