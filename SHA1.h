#pragma once
#include <iostream>
#include <vector>
#include <bitset>

#define BLOCK_SZ 512
#define SUBBLOCK_SZ 448

void PrintVector(const std::vector<bool> &Vector);
void PrintHexVector(const std::vector<bool>& Vector, const int BlockSz = 32);

uint32_t VecToUInt32(const std::vector<bool>& Vector);
std::vector<bool> StrToVec(const std::string& String);

uint32_t RotateLShift(uint32_t Num, int Steps = 1);

std::vector<bool> GetHash(const std::vector<bool> &Input);