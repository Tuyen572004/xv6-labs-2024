#include "user.h"
#include "../kernel/sysinfo.h"

int main() {
    struct sysinfo info;

    if (sysinfo(&info) < 0) {
        printf("sysinfotest: sysinfo failed\n");
        exit(1);
    }
    
    printf("Free memory: %ld bytes\n", info.freemem); 
    printf("Active processes: %ld\n", info.nproc);  
    printf("sysinfotest: OK\n");
    exit(0);
}
