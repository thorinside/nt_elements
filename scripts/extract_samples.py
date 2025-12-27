#!/usr/bin/env python3
"""
Extract wavetable and noise samples from Elements resources.cc into WAV files.

Extracts smp_sample_data into 9 separate WAV files (one per wavetable region)
and smp_noise_sample into 1 WAV file, all in 16-bit PCM mono @ 48kHz format.

Usage:
    python3 scripts/extract_samples.py [--force] [--source PATH]

Options:
    --force     Overwrite existing files
    --source    Path to resources.cc (defaults to external/mutable-instruments/elements/resources.cc)
"""

import argparse
import os
import re
import struct
import sys
import wave
from pathlib import Path


# Constants
SAMPLE_RATE = 48000  # disting NT native rate
NUM_CHANNELS = 1     # mono
SAMPLE_WIDTH = 2     # 16-bit (2 bytes)

# Expected boundaries from the original Elements resources.cc
# These define the start of each wavetable region
EXPECTED_BOUNDARIES = [0, 17099, 20852, 30369, 63050, 85807, 95952, 106297, 117606, 128013]


def parse_cpp_int16_array(content: str, array_name: str) -> list[int]:
    """
    Extract int16_t values from a C++ array definition.

    Args:
        content: Full file content as string
        array_name: Name of the array to extract (e.g., 'smp_sample_data')

    Returns:
        List of integer values from the array
    """
    # Match pattern like: const int16_t smp_sample_data[] = { ... };
    # The array can span many lines and contain comments
    pattern = rf'const\s+int16_t\s+{array_name}\s*\[\s*\]\s*=\s*\{{'

    match = re.search(pattern, content)
    if not match:
        raise ValueError(f"Array '{array_name}' not found in source file")

    # Find the matching closing brace
    start = match.end()
    brace_count = 1
    end = start

    while brace_count > 0 and end < len(content):
        if content[end] == '{':
            brace_count += 1
        elif content[end] == '}':
            brace_count -= 1
        end += 1

    # Extract content between braces
    array_content = content[start:end-1]

    # Remove C-style comments
    array_content = re.sub(r'/\*.*?\*/', '', array_content, flags=re.DOTALL)
    # Remove C++ style comments
    array_content = re.sub(r'//.*$', '', array_content, flags=re.MULTILINE)

    # Parse integer values (handles negative numbers and whitespace)
    values = []
    for match in re.finditer(r'-?\d+', array_content):
        values.append(int(match.group()))

    return values


def parse_cpp_size_t_array(content: str, array_name: str) -> list[int]:
    """
    Extract size_t values from a C++ array definition.

    Args:
        content: Full file content as string
        array_name: Name of the array to extract (e.g., 'smp_boundaries')

    Returns:
        List of integer values from the array
    """
    # Match pattern like: const size_t smp_boundaries[] = { ... };
    pattern = rf'const\s+size_t\s+{array_name}\s*\[\s*\]\s*=\s*\{{'

    match = re.search(pattern, content)
    if not match:
        raise ValueError(f"Array '{array_name}' not found in source file")

    # Find the matching closing brace
    start = match.end()
    brace_count = 1
    end = start

    while brace_count > 0 and end < len(content):
        if content[end] == '{':
            brace_count += 1
        elif content[end] == '}':
            brace_count -= 1
        end += 1

    # Extract content between braces
    array_content = content[start:end-1]

    # Remove comments
    array_content = re.sub(r'/\*.*?\*/', '', array_content, flags=re.DOTALL)
    array_content = re.sub(r'//.*$', '', array_content, flags=re.MULTILINE)

    # Parse integer values
    values = []
    for match in re.finditer(r'\d+', array_content):
        values.append(int(match.group()))

    return values


def write_wav(filepath: Path, samples: list[int], sample_rate: int = SAMPLE_RATE) -> None:
    """
    Write samples to a WAV file.

    Args:
        filepath: Output file path
        samples: List of 16-bit signed integer samples
        sample_rate: Sample rate in Hz (default 48000)
    """
    filepath.parent.mkdir(parents=True, exist_ok=True)

    with wave.open(str(filepath), 'w') as wav:
        wav.setnchannels(NUM_CHANNELS)
        wav.setsampwidth(SAMPLE_WIDTH)
        wav.setframerate(sample_rate)
        # Pack samples as signed 16-bit little-endian integers
        wav.writeframes(struct.pack(f'<{len(samples)}h', *samples))


def extract_samples(source_path: Path, output_dir: Path, force: bool = False) -> dict[str, Path]:
    """
    Extract all samples from resources.cc into WAV files.

    Args:
        source_path: Path to resources.cc
        output_dir: Output directory for WAV files
        force: Overwrite existing files if True

    Returns:
        Dictionary mapping output name to file path
    """
    # Check if all files already exist (idempotency)
    expected_files = [output_dir / f"wavetable_{i:02d}.wav" for i in range(9)]
    expected_files.append(output_dir / "noise.wav")

    if not force and all(f.exists() for f in expected_files):
        print(f"All {len(expected_files)} sample files already exist in {output_dir}")
        print("Use --force to regenerate")
        return {f.stem: f for f in expected_files}

    # Read source file
    print(f"Reading source file: {source_path}")
    with open(source_path, 'r') as f:
        content = f.read()

    # Parse arrays
    print("Parsing smp_boundaries array...")
    boundaries = parse_cpp_size_t_array(content, 'smp_boundaries')
    print(f"  Found {len(boundaries)} boundary values: {boundaries}")

    # Validate boundaries match expected values
    if boundaries != EXPECTED_BOUNDARIES:
        print(f"  WARNING: Boundaries differ from expected values")
        print(f"  Expected: {EXPECTED_BOUNDARIES}")

    print("Parsing smp_sample_data array...")
    sample_data = parse_cpp_int16_array(content, 'smp_sample_data')
    print(f"  Found {len(sample_data)} samples")

    print("Parsing smp_noise_sample array...")
    noise_data = parse_cpp_int16_array(content, 'smp_noise_sample')
    print(f"  Found {len(noise_data)} samples")

    # Extract wavetables
    output_files = {}

    for i in range(len(boundaries) - 1):
        start = boundaries[i]
        end = boundaries[i + 1]
        wavetable = sample_data[start:end]

        output_path = output_dir / f"wavetable_{i:02d}.wav"

        if output_path.exists() and not force:
            print(f"  Skipping {output_path.name} (already exists)")
        else:
            write_wav(output_path, wavetable)
            print(f"  Created {output_path.name}: {len(wavetable)} samples ({len(wavetable)/SAMPLE_RATE:.3f}s)")

        output_files[output_path.stem] = output_path

    # Extract noise sample
    noise_path = output_dir / "noise.wav"
    if noise_path.exists() and not force:
        print(f"  Skipping {noise_path.name} (already exists)")
    else:
        write_wav(noise_path, noise_data)
        print(f"  Created {noise_path.name}: {len(noise_data)} samples ({len(noise_data)/SAMPLE_RATE:.3f}s)")

    output_files[noise_path.stem] = noise_path

    return output_files


def main():
    parser = argparse.ArgumentParser(
        description='Extract Elements wavetable and noise samples to WAV files'
    )
    parser.add_argument(
        '--force', '-f',
        action='store_true',
        help='Overwrite existing files'
    )
    parser.add_argument(
        '--source', '-s',
        type=Path,
        default=None,
        help='Path to resources.cc (default: external/mutable-instruments/elements/resources.cc)'
    )
    parser.add_argument(
        '--output', '-o',
        type=Path,
        default=None,
        help='Output directory (default: samples/elements)'
    )

    args = parser.parse_args()

    # Determine project root (script is in scripts/ subdirectory)
    script_dir = Path(__file__).parent
    project_root = script_dir.parent

    # Set default paths
    if args.source is None:
        args.source = project_root / 'external' / 'mutable-instruments' / 'elements' / 'resources.cc'

    if args.output is None:
        args.output = project_root / 'samples' / 'elements'

    # Validate source file exists
    if not args.source.exists():
        print(f"Error: Source file not found: {args.source}", file=sys.stderr)
        sys.exit(1)

    print(f"Elements Sample Extractor")
    print(f"========================")
    print(f"Source: {args.source}")
    print(f"Output: {args.output}")
    print(f"Force:  {args.force}")
    print()

    try:
        output_files = extract_samples(args.source, args.output, args.force)
        print()
        print(f"Extraction complete: {len(output_files)} files")
        return 0
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
