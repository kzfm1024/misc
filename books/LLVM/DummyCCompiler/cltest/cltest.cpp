#include <llvm/Support/CommandLine.h>
#include <cstdio>

namespace {
    llvm::cl::opt<std::string> OutputFile(
        "o",
        llvm::cl::desc("Specify output file"),
        llvm::cl::value_desc("filename"),
        llvm::cl::init("-")
        );
}

int main(int argc, char **argv)
{
    llvm::cl::ParseCommandLineOptions(argc, argv, "CommandLine Sample\n");
    fprintf(stderr, "output file : %s\n", OutputFile.c_str());
    return 0;
}

