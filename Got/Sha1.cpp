#include "Sha1.h"
#include <sstream>
#include <iomanip>
#include <cstring>

Sha1::Sha1() : messageLengthBits(0), h{ 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0 } {}

void Sha1::update(const std::string& data) {
    update(std::vector<uint8_t>(data.begin(), data.end()));
}

void Sha1::update(const std::vector<uint8_t>& data) {
    messageLengthBits += static_cast<uint64_t>(data.size()) * 8;
    buffer.insert(buffer.end(), data.begin(), data.end());
    processBuffer();
}

void Sha1::processBuffer() {
    while (buffer.size() >= 64) {
        processBlock(buffer.data());
        buffer.erase(buffer.begin(), buffer.begin() + 64);
    }
}

void Sha1::padMessage() {
    buffer.push_back(0x80);
    while ((buffer.size() + 8) % 64 != 0) {
        buffer.push_back(0x00);
    }

    for (int i = 7; i >= 0; --i) {
        buffer.push_back(static_cast<uint8_t>((messageLengthBits >> (i * 8)) & 0xFF));
    }

    processBuffer();
}

std::string Sha1::final() {
    padMessage();

    std::ostringstream result;
    for (uint32_t part : h) {
        result << std::hex << std::setw(8) << std::setfill('0') << part;
    }
    return result.str();
}

std::string Sha1::hash(const std::string& data) {
    Sha1 sha;
    sha.update(data);
    return sha.final();
}

void Sha1::processBlock(const uint8_t block[64]) {
    uint32_t w[80];
    for (int i = 0; i < 16; ++i) {
        w[i] = (block[i * 4 + 0] << 24)
            | (block[i * 4 + 1] << 16)
            | (block[i * 4 + 2] << 8)
            | (block[i * 4 + 3]);
    }
    for (int i = 16; i < 80; ++i) {
        w[i] = (w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16]);
        w[i] = (w[i] << 1) | (w[i] >> 31); // rotate left
    }

    uint32_t a = h[0];
    uint32_t b = h[1];
    uint32_t c = h[2];
    uint32_t d = h[3];
    uint32_t e = h[4];

    for (int i = 0; i < 80; ++i) {
        uint32_t f, k;
        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        }
        else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        }
        else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        }
        else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }
        uint32_t temp = ((a << 5) | (a >> 27)) + f + e + k + w[i];
        e = d;
        d = c;
        c = (b << 30) | (b >> 2);
        b = a;
        a = temp;
    }

    h[0] += a;
    h[1] += b;
    h[2] += c;
    h[3] += d;
    h[4] += e;
}