#include "kernel/types.h"
#include "user/user.h"

void sieve(int p_left[2]) {
    int prime, num;

    close(p_left[1]);

    if (read(p_left[0], &prime, sizeof(prime)) == 0) {
        close(p_left[0]);
        exit(0); 
    }

    printf("prime %d\n", prime);

    int p_right[2];
    pipe(p_right);

    if (fork() == 0) {

        close(p_left[0]);
        sieve(p_right);
    } else {

        close(p_right[0]); 

        while (read(p_left[0], &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(p_right[1], &num, sizeof(num));
            }
        }

        close(p_left[0]);
        close(p_right[1]);
        wait(0); 
        exit(0); 
    }
}

int main() {

    int p_initial[2];
    pipe(p_initial);

    if (fork() == 0) {
        sieve(p_initial);
    } else {

        close(p_initial[0]); 

        for (int i = 2; i <= 280; i++) {
            write(p_initial[1], &i, sizeof(i));
        }

        close(p_initial[1]);  
        wait(0);    
        exit(0);
    }
}
