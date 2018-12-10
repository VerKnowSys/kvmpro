#include "kvmpro.h"


//
// kvmpro - memory leak utility check
//

int main(int argc, char const *argv[]) {

    for (int i = 0; i < 500; i++) {
        get_process_usage(i + 1);
    }

    for (int j = 0; j < 500; j++) {
        get_process_usage_short(j + 1);
    }

    return 0;
}
