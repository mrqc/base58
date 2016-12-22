#include "Base58.hpp"
#include <string>
#include <iostream>
#include <cstddef>
#include <stddef.h>
#include <string>
#include <string.h>

using namespace std;

namespace std
{
    const char* kLessConfusingChars = "123456789abcdefghijkmnopqrstuvwxyzABCDEFGHJKLMNPQRSTUVWXYZ";
    const int kMaxBase58Length = 11 + 1; // __int64(-1) => JPwcyDCgEup
    const int kMaxUInt64Length = 20;     // __int64(-1) => 18446744073709551615

    void swap(char& a, char& b) {
        char tmp = a;
        a = b;
        b = tmp;
    }

    void reverse(char* s, size_t n) {
        size_t count = n / 2;
        size_t head = 0, tail = n;
        while (count--) {
            swap(s[head++], s[--tail]);
        }
    }

    int decode_char(unsigned char ch) {
        if (ch >= '1' && ch <= '9')
            return ch - '1';
        if (ch >= 'a' && ch <= 'k')
            return ch - 'a' + 9;
        if (ch >= 'm' && ch <= 'z')
            return ch - 'm' + 20;
        if (ch >= 'A' && ch <= 'H')
            return ch - 'A' + 34;
        if (ch >= 'J' && ch <= 'N')
            return ch - 'J' + 42;
        if (ch >= 'P' && ch <= 'Z')
            return ch - 'P' + 47;
        return -1;
    }
}

namespace base58
{
    size_t encode(uint64_t id_num, char* output, size_t output_len)
    {
        if (output == 0)
            return 0;
        if (output_len < kMaxBase58Length)
            return 0;
        size_t output_wpos = 0;
        uint64_t num;
        int remainder;
        while (id_num > 0) {
            num = id_num / 58;
            remainder = id_num % 58;
            if (output_wpos < output_len) {
                output[output_wpos++] = kLessConfusingChars[remainder];
            }
            else {
                output[0] = '\0';
                return 0;
            }
            id_num = num;
        }
        output[output_wpos] = '\0';
        reverse(output, output_wpos);
        return output_wpos;
    }

    bool decode(const char* encoded, size_t encoded_len, uint64_t* output)
    {
        if (encoded == 0 || encoded_len == 0) 
            return 0;
        if (encoded_len == -1)
            encoded_len = strlen(encoded);
        if (encoded_len >= kMaxBase58Length) 
            return 0;
        if (output == 0)
            return 0;
        uint64_t output_num = 0;
        size_t unprocessed = encoded_len;
        while (--unprocessed) {
            int idx = encoded_len - unprocessed - 1;
            int num = decode_char(encoded[idx]);
            if (num == -1) {
                return 0;
            }
            output_num += num;
            output_num *= 58;
        }

        int remainder = decode_char(encoded[encoded_len-1]);
        if (remainder == -1) {
            return 0;
        }
        *output = output_num + remainder;
        return true;
    }
}
