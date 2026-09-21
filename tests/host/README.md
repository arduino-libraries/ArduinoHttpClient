# Response parser regression

Use a local checkout of [ArduinoCore-API](https://github.com/arduino/ArduinoCore-API)
for the production String/Stream/Client interfaces and its host conversion helpers.
The test transport supplies in-memory HTTP response bytes; it does not emulate a
Wi-Fi chip or prove behavior on physical hardware.

From this repository root, set `CORE_API` to that checkout and run:

```sh
c++ -std=c++11 -fsanitize=address,undefined -Itests/host -I"$CORE_API" \
  -idirafter "$CORE_API/api" -Isrc src/HttpClient.cpp src/b64.cpp \
  tests/host/response_headers.cpp "$CORE_API"/api/String.cpp \
  "$CORE_API"/api/Print.cpp "$CORE_API"/api/Stream.cpp \
  "$CORE_API"/api/IPAddress.cpp "$CORE_API"/test/src/itoa.cpp \
  "$CORE_API"/test/src/dtostrf.cpp -o /tmp/http-response-headers
/tmp/http-response-headers
```

`-idirafter` avoids shadowing the system `string.h` with Arduino's `String.h`
on case-insensitive filesystems. The Core API emits existing weak-main warnings
with Clang on macOS.
