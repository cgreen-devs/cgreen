#!/bin/sh
# Bump the project version consistently across the three places that carry it:
#   CMakeLists.txt  -- the single source of truth (current version is read here)
#   include/cgreen/cgreen.h  -- the public CGREEN_VERSION* macros
#   debian/control  -- the packaging Version field
#
# Usage:
#   tools/bump-version.sh X.Y.Z     set an explicit version
#   tools/bump-version.sh --patch   increment patch        (X.Y.Z -> X.Y.Z+1)
#   tools/bump-version.sh --minor   increment minor        (X.Y.Z -> X.Y+1.0)
#   tools/bump-version.sh --major   increment major        (X.Y.Z -> X+1.0.0)
#
# The version is propagated, nothing is committed or tagged -- review the diff,
# then commit and tag yourself.
set -eu

cd "$(dirname "$0")/.."

CMAKE=CMakeLists.txt
HEADER=include/cgreen/cgreen.h
CONTROL=debian/control

# Current version: the single source of truth is CMakeLists.txt (not the last
# git tag, so a bump works even before the previous one has been tagged).
cur_major=$(sed -n 's/^set(APPLICATION_VERSION_MAJOR "\([0-9]*\)").*/\1/p' "$CMAKE")
cur_minor=$(sed -n 's/^set(APPLICATION_VERSION_MINOR "\([0-9]*\)").*/\1/p' "$CMAKE")
cur_patch=$(sed -n 's/^set(APPLICATION_VERSION_PATCH "\([0-9]*\)").*/\1/p' "$CMAKE")
if [ -z "$cur_major" ] || [ -z "$cur_minor" ] || [ -z "$cur_patch" ]; then
    echo "ERROR: could not read current version from $CMAKE" >&2
    exit 1
fi

case "${1:-}" in
    --patch) major=$cur_major;          minor=$cur_minor;          patch=$((cur_patch + 1)) ;;
    --minor) major=$cur_major;          minor=$((cur_minor + 1));  patch=0 ;;
    --major) major=$((cur_major + 1));  minor=0;                   patch=0 ;;
    [0-9]*.[0-9]*.[0-9]*)
        major=${1%%.*}; rest=${1#*.}; minor=${rest%%.*}; patch=${rest#*.}
        case "$patch" in
            ''|*[!0-9]*) echo "ERROR: version must be X.Y.Z (digits only)" >&2; exit 1 ;;
        esac ;;
    *) echo "Usage: $0 X.Y.Z | --patch | --minor | --major" >&2; exit 1 ;;
esac

new="$major.$minor.$patch"
echo "Bumping $cur_major.$cur_minor.$cur_patch -> $new"

edit() {  # edit <file> <sed-script>
    sed -E "$2" "$1" > "$1.bump.tmp" && mv "$1.bump.tmp" "$1"
}

edit "$CMAKE" 's/(APPLICATION_VERSION_MAJOR ")[0-9]+(")/\1'"$major"'\2/;
               s/(APPLICATION_VERSION_MINOR ")[0-9]+(")/\1'"$minor"'\2/;
               s/(APPLICATION_VERSION_PATCH ")[0-9]+(")/\1'"$patch"'\2/'

edit "$HEADER" 's/(#define CGREEN_VERSION ")[0-9]+\.[0-9]+\.[0-9]+(")/\1'"$new"'\2/;
                s/(#define CGREEN_VERSION_MAJOR )[0-9]+/\1'"$major"'/;
                s/(#define CGREEN_VERSION_MINOR )[0-9]+/\1'"$minor"'/;
                s/(#define CGREEN_VERSION_PATCH )[0-9]+/\1'"$patch"'/'

edit "$CONTROL" 's/^(Version: ).*/\1'"$new"'/'

echo "Updated:"
echo "  $CMAKE:";   grep -E 'APPLICATION_VERSION_(MAJOR|MINOR|PATCH) "' "$CMAKE"  | sed 's/^/    /'
echo "  $HEADER:";  grep -E '#define CGREEN_VERSION'                    "$HEADER" | sed 's/^/    /'
echo "  $CONTROL:"; grep -E '^Version:'                                 "$CONTROL" | sed 's/^/    /'
