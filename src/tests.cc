#include "kvmpro.h"


//
// kvmpro - test suite - PoC
//

int main(int argc, char const *argv[]) {
    cout << "TESTS: MAIN(get_process_usage())" << endl;
    cout << Kvmpro::get_process_usage(0) << endl;

    cout << "TESTS: MAIN(get_process_usage_short())" << endl;
    cout << Kvmpro::get_process_usage_short(0) << endl;
    return 0;
}
