#include "user.h"
#include "../kernel/sysinfo.h"

int main() {
    struct sysinfo info;

    if (sysinfo(&info) < 0) {
        printf("sysinfotest: sysinfo failed\n");
        exit(1);
    }
    
    // Sửa lại định dạng từ %d thành %ld cho uint64_t
    printf("Free memory: %ld bytes\n", info.freemem);  // %ld cho uint64_t
    printf("Active processes: %ld\n", info.nproc);     // %ld cho uint64_t
    printf("sysinfotest: OK\n");
    exit(0);
}
