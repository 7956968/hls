#!/bin/bash

main() {
    python3 ../libiwu/scripts/test_runner.py \
        --binaries bin/dash_test_core bin/dash_test_mpd \
        --valgrind \
        --repeat 1
}

main "$@"

exit $?
