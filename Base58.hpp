#include<stddef.h>
#include<stdint.h>

using namespace std;

namespace base58 {
    size_t encode(uint64_t id_num, char* output, size_t output_len);
    bool decode(const char* encoded, size_t encoded_len, uint64_t* output);
}
