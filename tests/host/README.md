# Base64 capacity regression

From the repository root:

```sh
c++ -std=c++11 -Wall -Wextra -Werror -fsanitize=address,undefined \
  -Isrc src/b64.cpp tests/host/base64_capacity.cpp -o /tmp/http-base64-capacity
/tmp/http-base64-capacity
```

Checks standard short vectors, every undersized output capacity, output canaries,
negative input lengths, and lengths whose result cannot fit the return type.
No board or network hardware is needed for this pure encoding routine.
