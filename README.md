Userland tests for linux-file-tracer
====================================

Prerequisites
-------------

You will need to create non-root user account (see `TESTUSER` variable in Makefile)
and make sure that `debugfs` is mounted under `/sys/kernel/debug/`.

Running
-------

```
cat /sys/kernel/debug/tracing/trace_pipe > /dev/null      # Ctrl-C after a while
make clean
make test                                                 # this expands tracing buffer
cat /sys/kernel/debug/tracing/trace > test.out
```

Copyright (c) 2013 Mateusz Machalica
