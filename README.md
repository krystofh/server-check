# Description

## Server-check

### Description

Testing availability of one or multiple servers using ping command executed in parallel.
The program `check-servers` sends a request to each server once each second. If the response is not obtained within the 1 s period,
it qualifies as `UNREACHABLE`, otherwise `OK`. The execution is done through threading using `pthread`.

Source code:

> check.c

### Usage

```bash
./check-servers IP1 IP2 IP999
```

## Alias-check

### Description

TODO

### Usage

TODO

## Sources

1. Unix Threads in C: https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2
2. https://www.geeksforgeeks.org/multithreading-in-c/
3. certain level of support from ChatGPT


