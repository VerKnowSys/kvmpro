#include "kvmpro.hpp"


//
// kvmpro - test suite - PoC
//

int main(int argc, char const *argv[]) {

    cout << "TESTS: C++: MAIN(get_process_usage_short())" << endl;
    cout << Kvmpro::get_process_usage_short(65) << endl;
    cout << "TESTS: C: MAIN(get_process_usage_short())" << endl;
    cout << get_process_usage_short(65) << endl;

    cout << "TESTS: C++: MAIN(get_process_usage())" << endl;
    cout << Kvmpro::get_process_usage(65) << endl;
    cout << "TESTS: C: MAIN(get_process_usage())" << endl;
    cout << get_process_usage(65) << endl;

    for (int i = 0; i < 1000; ++i) {
        if (i % 100 == 0)
            cout << "AT: " << i << ", of: " << 1000 << endl;
        get_process_usage(0);
        get_process_usage_short(0);
    }

    return 0;
}
