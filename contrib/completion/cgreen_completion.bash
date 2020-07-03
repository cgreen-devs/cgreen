#/usr/bin/env bash
#
# Contributed by Yavor Lulchev @RookieWookiee
# Improved a bit by @thoni56

# Thanks to https://stackoverflow.com/a/57243443/204658
_removeFromArray() {
    arrayName="$1"
    arrayNameAt="$arrayName"'[@]'
    removeValue="$2"
    mapfile -d '' -t "$arrayName" < <(
        printf %s\\0 "${!arrayNameAt}" | grep -zvFx -- "$removeValue")
}

_discover_tests()
{
    local SUT="$(nm -f posix $word | grep -o -E 'CgreenSpec\w*?\b' | awk -F '__' '{ print $2 }' | uniq)"
    local test_names=( $(nm -f posix $word | grep -o -E 'CgreenSpec\w*?\b' | sed -e 's/CgreenSpec__[a-zA-Z0-9]\+\?__//' -e 's/__$//') )

    if test $SUT = "default" ; then
        tests+=( ${test_names[@]} )
    else
        local prefix="$SUT\\:"
        tests+=( ${test_names[@]/#/$prefix} )
    fi
}

_cgreen_runner_completion()
{
    local options libraries tests
    options=("--colours" "--no-colours" "--xml" "--suite" "--verbose" "--no-run" "--help" " --version")
    libraries=( *.so )
    tests=()

    # Look for words in the command given so far
    for word in ${COMP_WORDS[@]}; do
        if echo $word | grep -q -E "\b\.so\b"; then
            _discover_tests
        fi
    done

    # Remove all suggestions already used
    for word in ${COMP_WORDS[@]}; do
        _removeFromArray options $word
        _removeFromArray libraries $word
        _removeFromArray tests $word
    done

    # Need to double the backslashes in the completion word for them to survive into matching
    completion_word=${COMP_WORDS[$COMP_CWORD]/\\/\\\\}
    COMPREPLY=($(compgen -W '$(printf "%s " ${options[@]} ${libraries[@]} ${tests[@]})' -- "$completion_word"))
}

_cgreen_debug_completion()
{
    local libraries tests
    libraries=( *.so )
    tests=()

    # Look for words in the command given so far
    for word in ${COMP_WORDS[@]}; do
        if echo $word | grep -q -E "\b\.so\b"; then
            _discover_tests
        fi
    done

    # Remove libraries and tests if already used (only one library and one test allowed)
    for word in ${COMP_WORDS[@]}; do
        if [[ $word == *".so" ]]; then
            # Only one library allowed
            libraries=()
        fi
        if [[ $word == *"\\:"* ]]; then
            if [[ "${COMP_WORDS[$COMP_CWORD]}" == "" ]]; then
                # Only one test allowed
                tests=()
            fi
        fi
    done

    # Need to double the backslashes in the completion word for them to survive into matching
    completion_word=${COMP_WORDS[$COMP_CWORD]/\\/\\\\}
    COMPREPLY=($(compgen -W '$(printf "%s " ${options[@]} ${libraries[@]} ${tests[@]})' -- "$completion_word"))
}

complete -o nosort -o dirnames -F _cgreen_runner_completion cgreen-runner
complete -o nosort -F _cgreen_debug_completion cgreen-debug
