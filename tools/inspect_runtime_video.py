"""Decode local emulator video evidence; screen/scenario acceptance stays manual."""
import argparse
import hashlib
import json
from pathlib import Path

import av

ROOT = Path(__file__).resolve().parents[1]


def inspect(session: Path) -> dict:
    session = session.resolve()
    session.relative_to(ROOT / 'build/ULUS10566/runtime/sessions')
    record = json.loads((session / 'session.json').read_text(encoding='utf-8'))
    if not record['stopped'] or not record['capture_video'] or record['debugger_enabled']:
        raise ValueError('Expected a stopped, video-capturing, offline session')
    videos = sorted((Path(record['memstick']) / 'PSP/VIDEO').glob('*.avi'))
    if not videos:
        raise ValueError('No recorded video; launch is not runtime evidence')
    evidence = session / 'evidence'
    evidence.mkdir(exist_ok=True)
    results = []
    for video in videos:
        count = 0
        last = None
        with av.open(str(video)) as container:
            stream = container.streams.video[0]
            codec = stream.codec_context.name
            for frame in container.decode(stream):
                count += 1
                last = frame
                if count in (1, 120, 300, 600, 1200):
                    frame.to_image().save(evidence / f'{video.stem}-frame-{count:06}.png')
            if last is None:
                raise ValueError('Video contains no decoded frames')
            image = last.to_image().convert('RGB')
            final = evidence / f'{video.stem}-last.png'
            image.save(final)
            results.append({
                'path': video.relative_to(ROOT).as_posix(),
                'sha256': hashlib.sha256(video.read_bytes()).hexdigest(),
                'codec': codec, 'decoded_frames': count,
                'width': image.width, 'height': image.height,
                'last_frame': final.relative_to(ROOT).as_posix(),
                'last_frame_rgb_sha256': hashlib.sha256(image.tobytes()).hexdigest(),
                'last_frame_time_seconds': float(last.time) if last.time is not None else None,
            })
    result = {'schema_version': 1, 'case': record['case'], 'videos': results,
              'scenario_acceptance': 'pending-human-or-agent-visual-inspection',
              'decoder': {'av': av.__version__}}
    (evidence / 'video.json').write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('session', type=Path)
    args = parser.parse_args()
    print(json.dumps(inspect(args.session), indent=2))
