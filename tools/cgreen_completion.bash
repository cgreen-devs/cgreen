# Contributed by Yavor Lulchev @RookieWookiee
# Improved by @thoni56

# Could not find a way to do these three in a general function (bash isn't my native language...)
_removeFromOptions() {
    new_array=()
    for value in "${options[@]}"
    do
        [[ $value != $1 ]] && new_array+=($value)
    done
    options=("${new_array[@]}")
    unset new_array
}

_removeFromLibraries() {
    new_array=()
    for value in "${libraries[@]}"
    do
        [[ $value != $1 ]] && new_array+=($value)
    done
    libraries=("${new_array[@]}")
    unset new_array
}

_removeFromTests() {
    new_array=()
    for value in "${tests[@]}"
    do
        [[ $value != $1 ]] && new_array+=($value)
    done
    tests=("${new_array[@]}")
    unset new_array
}

_discover_tests()
{
    if test -f $word ; then
        local raw=$(nm -f posix $word | grep -o -E 'CgreenSpec\w*?\b')
        local specs=( $(echo "$raw" | sed 's/^CgreenSpec__//g' | sed 's/__$//g') )
        # specs should be an array with all <SUT>__<testname> from the library in $word
        local SUTs=( $(printf "%s\n" "${specs[@]}" | awk -F '__' '{ print $1 }' | sort | uniq) )
        # SUTs should now contain all SUTs present in the library

        for SUT in "${SUTs[@]}"; do
            if test $SUT = "default" ; then
                tests+=( ${test_names[@]} )
            else
                sut_tests=()
                for spec in "${specs[@]}"; do
                    if [[ $spec == "$SUT"* ]] ; then
                        sut_tests+=( $spec )
                    fi
                done
                tests+=( ${sut_tests[@]/__/:} )
            fi
        done
    fi
}

case $OSTYPE in
    darwin* )
        LIBEXT=dylib
        ;;
    linux* )
        LIBEXT=so
        ;;
    cygwin )
        LIBEXT=dll
        ;;
esac

_cgreen_runner_completion()
{
    local options libraries tests
    options=("--colours" "--no-colours" "--xml" "--suite" "--verbose" "--no-run" "--help" " --version")
    libraries=()
    tests=()

    # Look for words in the command given so far
    for word in ${COMP_WORDS[@]}; do
        # Matching loadable libraries?
        if compgen -G "$word*.$LIBEXT" > /dev/null; then
            # Add that pattern
            libraries+=($word*.$LIBEXT)
        fi
        # Is it a library?
        if echo $word | grep -q -E "\b\.$LIBEXT\b"; then
            _discover_tests
        fi
    done

    # Remove all suggestions already used
    for word in ${COMP_WORDS[@]}; do
        if [[ "$word" != "${COMP_WORDS[0]}" ]] ; then
            _removeFromOptions $word
            _removeFromLibraries $word
            _removeFromTests $word
        fi
    done

    # Need to double the backslashes in the completion word for them to survive into matching
    completion_word=${COMP_WORDS[$COMP_CWORD]/\\/\\\\}
    COMPREPLY=($(compgen -W '$(printf "%s " ${options[@]} ${libraries[@]} ${tests[@]})' -- "$completion_word"))
}

_cgreen_debug_completion()
{
    local libraries tests
    libraries=()
    tests=()

    # Look for words in the command given so far
    for word in ${COMP_WORDS[@]}; do
        # Matching loadable libraries?
        if compgen -G "$word*.$LIBEXT" > /dev/null; then
            # Add that pattern
            libraries+=($word*.$LIBEXT)
        fi
        if echo $word | grep -q -E "\b\.$LIBEXT\b"; then
            _discover_tests
        fi
    done

    # Remove libraries and tests if already used (only one library and one test allowed)
    for word in ${COMP_WORDS[@]}; do
        if [[ $word == *".$LIBEXT" ]]; then
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

complete -o nosort -o dirnames -A directory -F _cgreen_runner_completion cgreen-runner
complete -o nosort -A directory -F _cgreen_debug_completion cgreen-debug
