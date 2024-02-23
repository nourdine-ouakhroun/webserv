#include <iostream>
#include <unistd.h>

int main() {
    // The program you want to execute
    const char* program = "/usr/bin/c++";

    // Arguments to the program, starting with the program name itself as per convention
    const char* args[] = {program, "Host/cgi/afficheName.py", nullptr};

    // Environment, passing NULL uses the current environment
    char* const env[] = {nullptr};

    // execve replaces the current process, so if it succeeds, it does not return

    // If execve returns, it means it failed
    
    std::cout << execve(program, const_cast<char* const*>(args), env) << std::endl;
    std::cout << "sss" ;
    return 0;
}
