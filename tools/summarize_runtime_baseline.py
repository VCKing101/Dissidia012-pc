"""Revalidate and report only the reviewed first-screen runtime checkpoint."""
import argparse
import json
from pathlib import Path

from inspect_runtime_video import inspect
from prepare_runtime_packages import ROOT, SOURCE_NAME, digest, validate_builds


def summarize(run_id):
    base = ROOT / 'build/ULUS10566/runtime'
    manifest = json.loads((base / 'packages/manifest.json').read_text())
    checkpoint = json.loads((ROOT / 'config/runtime-checkpoints.json').read_text())
    if validate_builds() != manifest['build_inputs']:
        raise ValueError('Runtime packages no longer reflect the current build proofs')
    source_hash = digest(ROOT / SOURCE_NAME)
    if source_hash != manifest['source']['sha256']:
        raise ValueError('Source ISO changed')
    packages = {p['label']: p for p in manifest['packages']}
    for package in packages.values():
        if digest(ROOT / package['path']) != package['sha256']:
            raise ValueError('Runtime package changed')
    cases = []
    for case in ('retail', 'original', 'rebuilt'):
        session = base / 'sessions' / case / run_id
        session.resolve().relative_to(base / 'sessions')
        launch = json.loads((session / 'session.json').read_text())
        run = json.loads((session / 'boot-check.json').read_text())
        expected_iso = source_hash if case == 'retail' else packages['original-decrypted' if case == 'original' else 'rebuilt']['sha256']
        if (launch['case'] != case or launch['iso_sha256'] != expected_iso
                or launch['ppsspp_version'] != checkpoint['ppsspp_version']
                or launch['graphics'] != checkpoint['graphics']
                or digest(Path(launch['exe'])) != launch['exe_sha256']
                or not run['alive_after_wait'] or run['scenario'] != 'boot-with-no-input'):
            raise ValueError(f'Invalid runtime evidence for {case}')
        video = inspect(session)
        if len(video['videos']) != 1:
            raise ValueError('Review multiple/resized recordings separately')
        frame = video['videos'][0]
        if (frame['codec'] != checkpoint['codec'] or frame['width'] != checkpoint['width']
                or frame['height'] != checkpoint['height'] or frame['last_frame_rgb_sha256'] != checkpoint['rgb_sha256']):
            raise ValueError(f'{case} did not reach the reviewed checkpoint; inspect its images')
        cases.append({'case': case, 'session': session.relative_to(ROOT).as_posix(),
                      'requested_wall_seconds': run['requested_seconds'],
                      'checkpoint_pass': True, 'video': frame,
                      'log_sha256': digest(Path(launch['log']))})
    result = {'schema_version': 1, 'target': 'ULUS10566 v1.00',
              'milestone': 'Local decrypted ISO packaging and first-screen boot',
              'source_iso_sha256': source_hash, 'source_iso_unchanged': True,
              'packages_byte_identical': manifest['packages_byte_identical'],
              'checkpoint': checkpoint, 'cases': cases,
              'scenario_status': {'boot_to_first_autosave_notice': 'passed-three-cases',
                                  'title': 'not-run', 'menus': 'not-run', 'battle': 'not-run',
                                  'in_game_save_load': 'not-run', 'data_install': 'not-run'},
              'full_runtime_baseline_complete': False,
              'remaining_obstacle': 'PPSSPP 1.20.4 startup debugger also enables network disc sharing; every debugger session needs verified isolation. Applied protection and deeper rebuilt-only checks are documented separately in protected-runtime-milestone.md.',
              'matching_c_changed': False}
    (ROOT / 'docs/decomp/runtime-baseline.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--run-id', default='offline-boot-v1')
    result = summarize(parser.parse_args().run_id)
    print(json.dumps({'checkpoint': result['checkpoint']['checkpoint'],
                      'passed': [c['case'] for c in result['cases']],
                      'full_runtime_baseline_complete': False}, indent=2))
