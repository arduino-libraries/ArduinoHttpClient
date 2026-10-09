#include "HttpClient.h"
#include <assert.h>
#include <stdio.h>
#include <string>

unsigned long millis() { static unsigned long clock = 0; return ++clock; }
void delay(unsigned long) {}

class ResponseClient : public Client {
  std::string response;
  size_t position = 0;
public:
  explicit ResponseClient(const std::string& value) : response(value) {}
  int connect(IPAddress, uint16_t) override { return 1; }
  int connect(const char*, uint16_t) override { return 1; }
  size_t write(uint8_t) override { return 1; }
  size_t write(const uint8_t*, size_t size) override { return size; }
  int available() override { return response.size() - position; }
  int read() override { return available() ? (unsigned char)response[position++] : -1; }
  int read(uint8_t* data, size_t size) override {
    size_t count = 0;
    while (count < size && available()) data[count++] = read();
    return count;
  }
  int peek() override { return available() ? (unsigned char)response[position] : -1; }
  void flush() override {}
  void stop() override {}
  uint8_t connected() override { return available() > 0; }
  operator bool() override { return true; }
};
int main() {
  for (const char* name : {"Content-Length", "content-length", "CONTENT-LENGTH", "cOnTeNt-LeNgTh"}) {
    ResponseClient transport(std::string("HTTP/1.1 200 OK\r\nX-Test: ignored\r\n") + name + ": 5\r\n\r\nhello");
    HttpClient http(transport, "example.test");
    assert(http.get("/") == 0);
    assert(http.responseStatusCode() == 200);
    assert(http.contentLength() == 5);
    assert(http.responseBody() == "hello");
  }
  for (const char* field : {"Transfer-Encoding: chunked", "transfer-encoding: chunked", "TRANSFER-ENCODING: CHUNKED", "tRaNsFeR-EnCoDiNg: ChUnKeD"}) {
    ResponseClient transport(std::string("HTTP/1.1 200 OK\r\n") + field + "\r\n\r\n5\r\nhello\r\n0\r\n\r\n");
    HttpClient http(transport, "example.test");
    assert(http.get("/") == 0);
    assert(http.responseStatusCode() == 200);
    assert(http.skipResponseHeaders() == 0);
    assert(http.isResponseChunked());
    assert(http.responseBody() == "hello");
  }
  puts("eight response-header casing cases passed");
}
