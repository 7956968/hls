#!/bin/bash

main() {
    clang-format \
        --style=file \
        -i \
        $(find libhls -name "*.h" | tr "\n" " ") \
        $(find libhls -name "*.cpp" | tr "\n" " ")

    return $?
}

main "$@"

exit $?

