#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>

int getch()
{
    int c;
    struct termios oldattr, newattr;

    tcgetattr(STDIN_FILENO, &oldattr); // 현재 터미널 설정 읽음
    newattr = oldattr;
    newattr.c_lflag &= ~(ICANON | ECHO);        // CANONICAL과 ECHO 끔
    newattr.c_cc[VMIN] = 1;                     // 최소 입력 문자 수를 1로 설정
    newattr.c_cc[VTIME] = 0;                    // 최소 읽기 대기 시간을 0으로 설정
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // 터미널에 설정 입력
    c = getchar();                              // 키보드 입력 읽음
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // 원래의 설정으로 복구
    return c;
}

int main(int argc, char *argv[])
{
    // printf("\033[%d;%df", 10,30);
    // printf("베팅하실 금액을 적어주세요(한도:200)");;
    // fflush(stdout);
    int fd = open("./test.dat", O_WRONLY | O_TRUNC);
    if (fd == -1)
    {
        perror("open_error");
        return -1;
    }
    int buf = 0;
    int size = 0;
    while (1)
    {
        buf = getch();

        if ( buf >= '0' && buf <= '9')
        {
            printf("%c", buf);
            fflush(stdout);
            size += write(fd, &buf, sizeof(char));
            if (size == -1)
            {
                perror("write_error ");
            }
            buf = 0;
        }
        if (buf == 10)
            break;
        if (buf == 127)
        {
            printf("\b \b");
            lseek(fd, (off_t)-1,SEEK_CUR);
            char* tmp = " ";
            size -= write(fd,&tmp, sizeof(char));
            lseek(fd, (off_t)-1,SEEK_CUR);
        }
    }

    close(fd);

    return 1;
}
