#include "kvmpro.hpp"


//
// kvmpro - memory leak utility check
//

const uint ROUNDS = 1024;


int main(int argc, char const *argv[]) {

    // C
    for (int ps1 = 0; ps1 < ROUNDS; ps1++) {
        get_process_usage(++ps1);
    }

    // C++
    for (int ps2 = 0; ps2 < ROUNDS; ps2++) {
        Kvmpro::get_process_usage(++ps2);
    }

    // C
    for (int ps3 = 1; ps3 < ROUNDS; ps3++) {
        get_process_usage_short(++ps3);
    }

    // C++
    for (int ps4 = 1; ps4 < ROUNDS; ps4++) {
        Kvmpro::get_process_usage_short(++ps4);
    }

    return 0;
}
