#include "kvmpro.h"


//
// kvmpro - memory leak utility check
//

int main(int argc, char const *argv[]) {

    for (int i = 0; i < 1000; i++) {
        get_process_usage_short(0);
        get_process_usage(0);
    }

    return 0;
}
