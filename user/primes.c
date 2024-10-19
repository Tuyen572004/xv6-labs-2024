#include "kernel/types.h"
#include "user/user.h"

void close_pipe(int *p)  // hàm đóng 2 đầu đọc ghi của đường ống
{
  close(p[0]);
  close(p[1]);
}

void primes() 
{
  int n, p, len;
  int fd[2];

  if ((len = read(0, &n, sizeof(int))) <= 0 || n < 2)  //kiểm tra hợp lệ
  {
    close(1);
    exit(0);
  }

  printf("prime %d\n", n);
  pipe(fd); 

  if (fork() == 0) 
  {
    close(0);
    dup(fd[0]);
    close_pipe(fd);
    primes();
  } 
  else 
  {
    close(1);
    dup(fd[1]);
    close_pipe(fd);
    while ((len = read(0, &p, sizeof(int))) > 0 && p >= 2) 
    {
      if (p % n != 0) 
      {
        write(1, &p, sizeof(int));
      }
    }
    if (len <= 0 || p < 2) 
    {
      close(1);
      exit(0);
    }
  } 
}

int main(void) 
{
  int fd[2]; // lưu đầu đọc và ghi của đường ống, f[0] là đọc, f[1] là ghi
  pipe(fd); //tạo đường ống

  if (fork() == 0)  // kiểm tra trong tiến trình con không
  {
    close(0); // đóng đầu vào tiêu chuẩn
    dup(fd[0]); // sao chép đầu đọc fd[0] vào vị trí stdin
    close_pipe(fd); // đóng ống
    primes();
  }
  else 
  {
    close(1); // đóng đầu ra tiêu chuẩn
    dup(fd[1]); // sao chép đầu ghi vào vị trí stdout vừa đóng
    close_pipe(fd);
    for (int i = 2; i <= 280; i++)  // gửi 280 số vào đầu ghi đã xác định ở trước
    {
      write(1, &i, sizeof(int));
    }
    close(1); //sau khi ghi xong đóng đầu ghi của đường ống
    wait(0); //  chờ tiến trình con kết thúc
  }

  exit(0);
}