#include "kvmpro.hpp"


#include <dlfcn.h>


typedef kvmpro_t (*kvmpro_t_t)(uid_t);

class LibDL {
    void* lib;
    kvmpro_t_t fptr;

public:
    LibDL();
    ~LibDL();
};


LibDL::LibDL() {
     lib = dlopen("/usr/lib/libkvmpro.so", RTLD_LAZY);
     fptr = (kvmpro_t_t)dlsym(lib, "get_process_usage_t");
     // cout << fptr(65).bytes;
}


LibDL::~LibDL() {
    dlclose(lib);
}


//
// kvmpro - test suite - libDL in loop test
//

int main(int argc, char const *argv[]) {

    for (int i = 0; i < 100000; ++i) {
        if (i % 10000 == 0)
            cout << "AT: " << i << ", of: " << 100000 << endl;

        auto a = new LibDL();
        delete a;
    }

    return 0;
}
