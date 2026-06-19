#!/usr/bin/env bash
set -euo pipefail

artifact_dir="$1"
artifact="$2"
docker_platform="$3"
php_image="$4"
smoke_script="$5"

artifact_dir="$(cd "$artifact_dir" && pwd)"
smoke_script="$(cd "$(dirname "$smoke_script")" && pwd)/$(basename "$smoke_script")"

mapfile -t files < <(find "$artifact_dir" -maxdepth 1 -type f -name '*.so' | sort)

if [ "${#files[@]}" -ne 1 ]; then
  echo "Expected exactly one downloaded .so artifact, found ${#files[@]}" >&2
  printf '  %s\n' "${files[@]}" >&2
  exit 1
fi

if [ "$(basename "${files[0]}")" != "$artifact" ]; then
  echo "Expected artifact $artifact, found $(basename "${files[0]}")" >&2
  exit 1
fi

set +e
output="$(docker run --rm \
  --platform "$docker_platform" \
  -v "${artifact_dir}:/artifacts:ro" \
  -v "${smoke_script}:/smoke.php:ro" \
  "$php_image" \
  php -d "extension=/artifacts/${artifact}" /smoke.php 2>&1)"
status=$?
set -e

printf '%s\n' "$output"

if [ "$status" -eq 0 ]; then
  echo "Smoke script was expected to fail on invariant violation" >&2
  exit 1
fi

for expected in \
  "constructed" \
  "invariant 1" \
  "valid" \
  "invariant 2" \
  "invalid" \
  "smoke invalid"
do
  if ! grep -Fq "$expected" <<<"$output"; then
    echo "Smoke output missing expected text: $expected" >&2
    exit 1
  fi
done

if grep -Fq "unreachable" <<<"$output"; then
  echo "Smoke script continued after expected invariant failure" >&2
  exit 1
fi
