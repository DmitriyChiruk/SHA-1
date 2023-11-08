#include "SHA1.h"


void PrintVector(const std::vector<bool> &Vector)
{
    for (auto i : Vector)
    {
        std::cout << i;
    }
    std::cout << std::endl;
}


void PrintHexVector(const std::vector<bool> &Vector, const int BlockSz)
{
    std::cout << "0x";
    for (int i = 0; i < Vector.size(); i+=BlockSz)
    {
        uint32_t tmp = 0;
        for (char j = 0; j < BlockSz; j++)
        {
            if (i + j >= Vector.size()) { break; }

            tmp |= Vector[i + j];
            if (j != BlockSz - 1) { tmp <<= 1; }
        }
        std::cout << std::hex << tmp;
    }
    std::cout << std::dec << std::endl;
}


uint32_t VecToUInt32(const std::vector<bool> &Vector)
{
    uint32_t res = Vector[0];

    for (char i = 1; i < Vector.size(); i++)
    {
        res |= Vector[i];
        if (i != Vector.size() - 1) { res <<= 1; }
    }

    return res;
}


std::vector<bool> StrToVec(const std::string &String)
{
    std::vector<bool> res;

    for (auto el : String)
    {
        for (char i = 7; i >= 0; i--)
        {
            res.push_back(el >> i & 1);
        }
    }

    return res;
}


uint32_t RotateLShift(uint32_t Num, int Steps)
{
    for (int i = 0; i < Steps; i++)
    {
        bool first_bit = Num >> 31;
        Num <<= 1;
        Num |= first_bit;
    }

    return Num;
}


void AppendVec(std::vector<bool>& Vector, uint32_t Num)
{
    for (int i = 0; i < 32; i++)
    {
        Vector.push_back(Num >> (31 - i) & 1);
    }
}


std::vector<bool> CreateHash(std::vector<bool> &Input)
{
    std::vector<bool> res;

    uint32_t h0 = 0x67452301, a = 0;
    uint32_t h1 = 0xEFCDAB89, b = 0;
    uint32_t h2 = 0x98BADCFE, c = 0;
    uint32_t h3 = 0x10325476, d = 0;
    uint32_t h4 = 0xC3D2E1F0, e = 0;

    // Iterate over blocks of 512 bits
    for (int k = 0; k < Input.size(); k += 512)
    {
        // SubVector of 512 bits
        std::vector<bool> OuterBlock(Input.begin() + k, Input.begin() + k + 512);
        
        // PrintVector(OuterBlock);

        // Create array of 80 blocks, 32 bits each
        std::vector<uint32_t> blocks(80, 0);

        // Set first 16 blocks as parts of 512 bits
        for (int i = 0; i < 16; i++)
        {
            std::vector<bool> tmp_vec(OuterBlock.begin() + i * 32, OuterBlock.begin() + (i + 1) * 32);
            blocks[i] = VecToUInt32(tmp_vec);
            
            // PrintVector(tmp_vec);
        }

        // Extend number of blocks to 80 using left rotate shift
        for (int j = 16; j <= 79; j++)
        {
            // Use XOR operations to form block to be shifted
            blocks[j] = blocks[j - 3] ^ blocks[j - 8] ^ blocks[j - 14] ^ blocks[j - 16];

            // Perform rotate shift
            blocks[j] = RotateLShift(blocks[j]);
        }

        // Init hash values for this part
        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;

        // Main loop
        for (uint8_t i = 0; i < 80; i++) {
            uint32_t F = 0, g = 0;
            if (i < 20) {
                F = (b & c) | ((~b) & d);
                g = 0x5A827999;
            }
            else if (i < 40) {
                F = b ^ c ^ d;
                g = 0x6ED9EBA1;
            }
            else if (i < 60) {
                F = (b & c) | (b & d) | (c & d);
                g = 0x8F1BBCDC;
            }
            else {
                F = b ^ c ^ d;
                g = 0xCA62C1D6;
            }

            // Update hash values
            uint32_t temp = RotateLShift(a, 5) + F + e + g + blocks[i];
            e = d;
            d = c;
            c = RotateLShift(b, 30);
            b = a;
            a = temp;
        }

        // std::cout << std::hex << "         0x";

        // Update results
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;

        // std::cout << h0 << h1 << h2 << h3 << h4 << std::dec << std::endl;

    }

    AppendVec(res, h0);
    AppendVec(res, h1);
    AppendVec(res, h2);
    AppendVec(res, h3);
    AppendVec(res, h4);

    return res;
}


std::vector<bool> GetHash(const std::vector<bool> &Input)
{
    std::vector<bool> res;

    // Set last bit as 1 and add k bits
    // (size - 64) mod 512 = 448

    // Set bit after the last one in input as 1
    std::vector<bool> PaddedInput(Input);
    PaddedInput.push_back(1);

    // Calculate size of padding
    uint64_t temp_size = SUBBLOCK_SZ - PaddedInput.size() % BLOCK_SZ;
    if (PaddedInput.size() % BLOCK_SZ > SUBBLOCK_SZ) temp_size += BLOCK_SZ;
    temp_size += 64;

    // Add padding to thee data
    PaddedInput.insert(PaddedInput.end(), temp_size, 0);

    // Set last 64 bits to correspond the size of data
    std::bitset<64> tmp(Input.size());

    std::cout << "Input:  " << Input.size() << " bits" << std::endl;
    std::cout << "Padded: " << PaddedInput.size() << " bits" << std::endl;

    for (int i = 1; i <= 64; i++)
    {
        PaddedInput[PaddedInput.size() - i] = tmp[i - 1];
    }

    res = CreateHash(PaddedInput);

	return res;
}