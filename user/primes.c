#include "kernel/types.h"
#include "user/user.h"

void close_pipe(int *p) 
{
    close(p[0]);
    close(p[1]);
}

void out()
{
    exit(0);
}

void primes() 
{
    int n, p, len;
    int fd[2];

    // Đọc số nguyên đầu tiên
    if ((len = read(0, &n, sizeof(int))) <= 0 || n < 2)  
    {
        out();
    }

    printf("prime %d\n", n);
    pipe(fd); 

    if (fork() == 0) 
    {
        // Tiến trình con: đọc từ fd[0] và gọi primes() cho các số nguyên tố tiếp theo
        close(0);
        dup(fd[0]);
        close_pipe(fd);
        primes(); // Cuộc gọi đệ quy cho tiến trình con
    } 
    else 
    {
        // Tiến trình cha: ghi vào fd[1]
        close(fd[0]);
        while ((len = read(0, &p, sizeof(int))) > 0 && p >= 2) 
        {
            if (p % n != 0) 
            {
                write(fd[1], &p, sizeof(int)); // Ghi vào fd[1]
            }
        }
        close(fd[1]); // Đóng fd[1] sau khi ghi xong
        wait(0); // Chờ tiến trình con kết thúc
    } 
}

int main(void) 
{
    int fd[2]; 
    pipe(fd); 

    if (fork() == 0)  
    {
        close(0);
        dup(fd[0]); 
        close_pipe(fd); 
        primes(); // Cuộc gọi đệ quy trong tiến trình con
    }
    else 
    {
        close(fd[0]); // Đóng đầu đọc
        for (int i = 2; i <= 280; i++)  
        {
            write(fd[1], &i, sizeof(int)); // Ghi vào fd[1]
        }
        close(fd[1]); // Đóng fd[1] sau khi ghi xong
        wait(0); 
    }

    exit(0);
}
