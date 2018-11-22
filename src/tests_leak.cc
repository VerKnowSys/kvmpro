#include "kvmpro.h"


//
// kvmpro - memory leak utility check
//

int main(int argc, char const *argv[]) {

    cout << "TESTS-LEAK: MAIN(get_process_usage_short())" << endl;
    for (int i = 0; i < 10000; i++) {
        get_process_usage_short(0);
        usleep(100);
    }

    cout << "TESTS-LEAK: MAIN(get_process_usage())" << endl;
    for (int i = 0; i < 10000; i++) {
        get_process_usage(0);
        usleep(100);
    }

    return 0;
}
