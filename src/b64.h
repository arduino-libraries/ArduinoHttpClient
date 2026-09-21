#ifndef b64_h
#define b64_h

// Returns the required size without writing when output capacity is too small.
// Returns -1 for a negative input length or an encoded size exceeding INT_MAX.
// The encoded output is not NUL-terminated.
int b64_encode(const unsigned char* aInput, int aInputLen, unsigned char* aOutput, int aOutputLen);

#endif
