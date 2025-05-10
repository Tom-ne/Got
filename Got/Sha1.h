#pragma once

#include <string>
#include <vector>
#include <array>

class Sha1 {
public:
    Sha1();
    void update(const std::string& data);
    void update(const std::vector<uint8_t>& data);
    std::string final();
    static std::string hash(const std::string& data);

private:
    void processBlock(const uint8_t block[64]);
    void padMessage();
    void processBuffer();

    uint64_t messageLengthBits;
    std::vector<uint8_t> buffer;

    std::array<uint32_t, 5> h;
};
