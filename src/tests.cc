#include "kvmpro.hpp"


//
// kvmpro - test suite - PoC
//

int main(int argc, char const *argv[]) {

    uid_t uid = 65;

    cout << "TESTS: C++: MAIN(get_process_usage_short())" << endl;
    cout << Kvmpro::get_process_usage_short(uid) << endl;
    cout << "TESTS: C: MAIN(get_process_usage_short())" << endl;
    cout << get_process_usage_short(uid) << endl;

    cout << "TESTS: C++: MAIN(get_process_usage())" << endl;
    cout << Kvmpro::get_process_usage(uid) << endl;
    cout << "TESTS: C: MAIN(get_process_usage())" << endl;
    cout << get_process_usage(uid) << endl;

    cout << "TESTS: C: MAIN(get_process_usage_tp())" << endl;
    cout << get_process_usage_tp(uid)->bytes << endl;
    cout << "TESTS: C: MAIN(get_process_usage_short_tp())" << endl;
    cout << get_process_usage_short_tp(uid)->bytes << endl;

    cout << "TESTS: C: MAIN(get_process_usage_t())" << endl;
    cout << get_process_usage_t(uid).bytes << endl;

    cout << "TESTS: C: MAIN(get_process_usage_short_t())" << endl;
    cout << get_process_usage_short_t(uid).bytes << endl;

    for (int i = 0; i < 1000; ++i) {
        if (i % 100 == 0)
            cout << "AT: " << i << ", of: " << 1000 << endl;

        kvmpro_t* a = get_process_usage_tp(uid);
        kvmpro_t* b = get_process_usage_short_tp(uid);
        assert(a != nullptr);
        assert(b != nullptr);
        destroy_kvmpro_tp(a);
        destroy_kvmpro_tp(b);
        get_process_usage_t(uid);
        get_process_usage_short_t(uid);
    }

    return 0;
}
