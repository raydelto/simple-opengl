#pragma once

#include <vector>

bool LoadTGA(const char *filename, std::vector<unsigned char> &data, unsigned short &width, unsigned short &height);
