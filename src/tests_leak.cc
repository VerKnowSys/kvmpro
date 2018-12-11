#include "kvmpro.h"


//
// kvmpro - memory leak utility check
//

int main(int argc, char const *argv[]) {

    for (int i = 1; i < 500; i++) {
        get_process_usage(i);
    }

    for (int j = 1; j < 500; j++) {
        get_process_usage_short(j);
    }

    return 0;
}
