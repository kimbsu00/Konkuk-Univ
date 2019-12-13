#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <termios.h>
#define BUFSIZE 1800
struct Object
{
    int score;
    char name[3];
};
int main(void)
{
    system("clear");
    char buffer[BUFSIZE];
    int cardNum;
    int fds = open("./intro.txt", O_RDONLY);
    int bytesread = read(fds, buffer, BUFSIZE);
    if (bytesread == -1)
    {
        perror("cannot read intro.txt");
        exit(0);
    }
    //printf("bytesread: %d\n",bytesread);
    buffer[bytesread - 1] = '\0';
    printf("\n\n\n\n%s\n", buffer);
    do
    {
        printf("\t\t\tEnter the number of games that you want (Integer between 1 to 52) : ");
        fflush(stdout);
        scanf("%d", &cardNum);
        while (getchar() != '\n');
    } while (cardNum > 52 || cardNum < 1);
    //ranking 출력
    struct Object player[10] = {
        0,
    };
    int fd;
    if ((fd = open("./rank.txt", O_RDONLY)) == -1)
        perror("open_rank.txt");

    char buf[1];
    int read_B;
    int score = 0;
    int index = 0;
    for (index = 0; index < 10; index++)
    {
        score = 0;
        for (int j = 0; j < 3; j++)
        {
            read_B = read(fd, buf, sizeof(char));
            if (read_B == 0)
                break;
            player[index].name[j] = buf[0];
        }
        if (read_B == 0)
            break;
        while (1)
        {
            read_B = read(fd, buf, sizeof(char));
            if (read_B == 0)
                break;
            if (buf[0] == '/')
            {
                break;
            }
            switch (buf[0])
            {
            case '0':
                score += 0;
                score *= 10;
                break;
            case '1':
                score += 1;
                score *= 10;
                break;
            case '2':
                score += 2;
                score *= 10;
                break;
            case '3':
                score += 3;
                score *= 10;
                break;
            case '4':
                score += 4;
                score *= 10;
                break;
            case '5':
                score += 5;
                score *= 10;
                break;
            case '6':
                score += 6;
                score *= 10;
                break;
            case '7':
                score += 7;
                score *= 10;
                break;
            case '8':
                score += 8;
                score *= 10;
                break;
            case '9':
                score += 9;
                score *= 10;
                break;
            default:
                break;
            }
        }
        score /= 10;
        player[index].score = score;
        if (read_B == 0)
            break;
    }
    system("clear");
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t========CURRENT=RANKING========\n");
    for (int i = 0; i < 10; i++)
    {
        printf("\t\t\t\t|      %2d) %3s : %5d        |\n", i + 1, player[i].name, player[i].score);
    }
    printf("\t\t\t\t========CURRENT=RANKING========");
    fflush(stdout);
    sleep(2);
    tcflush(0, TCIFLUSH);
    exit(cardNum);
    return 0;
}
