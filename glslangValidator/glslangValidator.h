#pragma once

#include <map>
#include <string>
#include <vector>


enum TFailCode {
    ESuccess = 0,
    EFailUsage,
    EFailCompile,
    EFailLink,
    EFailCompilerCreate,
    EFailThreadCreate,
    EFailLinkerCreate
};

TFailCode glslangValidator(int argc, char* argv[]);
void AddGlslangSource(const char* filename, const char* source);
std::map<std::string, std::vector<unsigned int>>& GetCompiledSpv();
