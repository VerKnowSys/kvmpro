#include "kvmpro.h"


//
// kvmpro - test suite - PoC
//

int main(int argc, char const *argv[]) {

    cout << "TESTS: MAIN(get_process_usage_short())" << endl;
    cout << get_process_usage_short(65) << endl;

    cout << "TESTS: MAIN(get_process_usage())" << endl;
    cout << get_process_usage(65) << endl;

    return 0;
}
