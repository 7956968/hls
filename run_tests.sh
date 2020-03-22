#!/bin/bash

main() {
    local root="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

    python3 ${root}/../libiwu/scripts/test_runner.py \
        --binaries ${root}/build/bin/hls-test \
        --iwtest_assets="${root}/libhls/test/assets" \
        "$@"
}

main "$@"

exit $?
