#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

typedef enum
{
   false,
   true
} bool;
struct Object
{
   int score;
   char name[3];
};
int getch()
{
   int c;
   struct termios oldattr, newattr;

   tcgetattr(STDIN_FILENO, &oldattr); // 현재 터미널 설정 읽음
   newattr = oldattr;
   newattr.c_lflag &= ~(ICANON | ECHO);        // CANONICAL과 ECHO 끔
   newattr.c_cc[VMIN] = 1;                     // 최소 입력 문자 수를 1로 설정
   newattr.c_cc[VTIME] = 0;                    // 최소 읽기 대기 시간을 0으로
   tcsetattr(STDIN_FILENO, TCSANOW, &newattr); // 터미널에 설정 입력
   c = getchar();                              // 키보드 입력 읽음
   tcsetattr(STDIN_FILENO, TCSANOW, &oldattr); // 원래의 설정으로 복구
   return c;
}
void gotoxy(int x, int y)
{
   printf("\033[%d;%df", y, x);
   fflush(stdout);
}
void printCard(int n, int x, int y)
{

   char *card[52] = {"♦ A", "♦ 2", "♦ 3", "♦ 4", "♦ 5", "♦ 6", "♦ 7", "♦ 8", "♦ 9", "♦ 10", "♦ J", "♦ Q", "♦ K", "♣ A", "♣ 2", "♣ 3", "♣ 4", "♣ 5", "♣ 6", "♣ 7", "♣ 8", "♣ 9", "♣ 10", "♣ J", "♣ Q", "♣ K", "♠ A", "♠ 2", "♠ 3", "♠ 4", "♠ 5", "♠ 6", "♠ 7", "♠ 8", "♠ 9", "♠ 10", "♠ J", "♠ Q", "♠ K", "♥ A", "♥ 2", "♥ 3", "♥ 4", "♥ 5", "♥ 6", "♥ 7", "♥ 8", "♥ 9", "♥ 10", "♥ J", "♥ Q", "♥ K"};
   //system("clear");
   gotoxy(x + 1, y);
   printf("--------");
   gotoxy(x + 1, y + 7);
   printf("--------");
   for (int i = 0; i < 6; i++)
   {
      gotoxy(x, y + 1 + i);
      printf("|");
      gotoxy(x + 9, y + 1 + i);
      printf("|");
   }
   gotoxy(x + 1, y + 1);
   if (n % 13 == 9)
   {
      printf("\033[1;30m\033[1;47m%s    ", card[n]);
   }
   else
   {
      printf("\033[1;30m\033[1;47m%s     ", card[n]);
   }
   for (int i = 0; i < 4; i++)
   {
      gotoxy(x + 1, y + 2 + i);
      printf("        ");
   }
   gotoxy(x + 1, y + 6);
   if (n % 13 == 9)
   {
      printf("    %s", card[n]);
   }
   else
   {
      printf("     %s", card[n]);
   }
   printf("\33[0m");
}
void printFace(int n)
{

   char *eye[3] = {"@", "-", "^"};
   gotoxy(11, 18);
   printf("%s     %s", eye[n], eye[n]);
   gotoxy(14, 19);
   printf("|");
   gotoxy(13, 21);
   printf("---");
   fflush(stdout);
}

void mkName(char *str)
{
   char a = 'A';
   char tmp;
   int cnt = 0;
   while (cnt != 3)
   {
      printf("\b%c", a);
      tmp = getch();

      if (tmp == 10)
      {
         str[cnt] = a;
         cnt++;
         printf(" ");
      }
      if (tmp == 27)
      {
         tmp = getch();
         if (tmp == 91)
         {
            tmp = getch();
            if (tmp == 68)
            { //왼
               a--;
               if (a == 'A' - 1)
               {
                  a = 'Z';
               }
            }
            else if (tmp == 67)
            { //오
               a++;
               if (a == 'Z' + 1)
               {
                  a = 'A';
               }
            }
         }
      }
   }
}
int main()
{
   int introChild = fork();
   int stat;
   int totalCard;
   if (!introChild)
      execl("./intro", "intro", NULL);
   else
   {
      wait(&stat);
   }
   totalCard = WEXITSTATUS(stat);
   int money = 100;
   bool isOpen[52] = {
       true,
   };
   int low[10] = {0, 1, 1, 1, 2, 2, 2, 2, 2, 2};
   int mid[10] = {0, 0, 1, 1, 1, 1, 1, 1, 2, 2};
   int high[10] = {0, 0, 0, 0, 0, 0, 1, 1, 1, 2};
   int i = 0;
   while (i != totalCard)
   {
      srand(time(NULL));
      system("clear");
      int comNum = rand() % 52;
      int userNum = rand() % 52;
      int comCardNum;
      int userCardNum;

      gotoxy(10, 33);
      printf("balance : %d", money);
      fflush(stdout);
      gotoxy(10, 34);
      printf("number of left cards : %d", totalCard - 1 - i);
      while (isOpen[comNum] == true)
      {
         comNum = rand() % 52;
      }
      isOpen[comNum] = true;
      while (isOpen[userNum] == true)
      {
         userNum = rand() % 52;
      }
      isOpen[userNum] = true;

      comCardNum = comNum % 13;
      userCardNum = userNum % 13;

      printCard(comNum, 10, 10);
      int faceNum = rand() % 10;
      switch (userCardNum / 4)
      {
      case 0:
         printFace(low[faceNum]);
         break;
      case 1:
         printFace(mid[faceNum]);
         break;
      default:
         printFace(high[faceNum]);
         break;
      }

      i++;
      int bet;
      while (1)
      {
         bet = 0;
         pid_t child_bet, child_time;
         child_bet = fork();
         if (child_bet == -1)
         {
            perror("AE");
         }
         if (child_bet == 0)
         {
            execl("./bet", "bet", NULL);
         }
         child_time = fork();
         if (child_time == -1)
            perror("AE");
         if (child_time == 0)
         {
            execl("./timer", "timer", NULL);
         }
         pid_t childpid;
         int INT_X;
         childpid = wait(&INT_X);

         //gotoxy(50,50);
         //printf("child_bet : %d \n", child_bet);
         //printf("child_time : %d \n", child_time);
         //printf("childpid : %d \n", childpid);

         if (childpid == child_bet)
            kill(child_time, SIGKILL);
         if (childpid == child_time)
            kill(child_bet, SIGKILL);
         pid_t PID = wait(&INT_X);

         int fd = open("./test.dat", O_RDONLY);
         if (fd == -1)
         {
            perror("open_error");
         }
         lseek(fd, 0, SEEK_SET);
         char bp[1];
         int bytesread;
         while (1)
         {
            bytesread = read(fd, bp, sizeof(char));
            if (bytesread == 0)
               break;
            switch (bp[0])
            {
            case '0':
               bet += 0;
               bet *= 10;
               break;
            case '1':
               bet += 1;
               bet *= 10;
               break;
            case '2':
               bet += 2;
               bet *= 10;
               break;
            case '3':
               bet += 3;
               bet *= 10;
               break;
            case '4':
               bet += 4;
               bet *= 10;
               break;
            case '5':
               bet += 5;
               bet *= 10;
               break;
            case '6':
               bet += 6;
               bet *= 10;
               break;
            case '7':
               bet += 7;
               bet *= 10;
               break;
            case '8':
               bet += 8;
               bet *= 10;
               break;
            case '9':
               bet += 9;
               bet *= 10;
               break;
            default:
               break;
            }
         }
         bet /= 10;
         close(fd);
         if (bet > money)
         {
            bet = money;
         }
         if (bet == 0)
         {
            money -= 10;
            printf("Bet Please(-10)");
            fflush(stdout);
            sleep(2);
            tcflush(0, TCIFLUSH);
            printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b                                                 ");
            gotoxy(10, 33);
            printf("balance : %d ", money);
            fflush(stdout);
            continue;
         }
         break;
      }

      printCard(userNum, 10, 22);
      gotoxy(10, 30);
      printf("                                                                                                ");
      if (userCardNum > comCardNum)
      {
         gotoxy(10, 31);
         printf("Great! :)");
         fflush(stdout);
         money += bet;
      }
      else
      {
         gotoxy(10, 31);
         printf("I'm Sorry :(");
         fflush(stdout);
         money -= bet;
      }
      sleep(3);
      tcflush(0, TCIFLUSH);
      if (money <= 0)
      {
         system("clear");
         printf("You Lose! Please try again!\n");
         return;
      }
   }
   gotoxy(10, 35);
   printf("total score : %d\n", money);
   gotoxy(10, 36);
   printf("register your nickname! : ");
   struct Object player[10] = {
       0,
   };
   char nickName[3];
   mkName(nickName);
   //printf("%c%c%c",nickName[0],nickName[1],nickName[2]);
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
   if (index != 10)
   {
      index++;
   }
   close(fd);
   if ((fd = open("./rank.txt", O_WRONLY | O_TRUNC)) == -1)
      perror("open_rank.txt");
   struct Object tmp;
   int pos;
   for (pos = 0; pos < index; pos++)
   {
      if (player[pos].score < money)
      {
         tmp = player[pos];
         struct Object o_tmp;
         o_tmp.score = money;
         for (int k = 0; k < 3; k++)
         {
            o_tmp.name[k] = nickName[k];
         }
         player[pos] = o_tmp;
         pos++;
         break;
      }
   }
   for (; pos < index; pos++)
   {
      if (pos + 1 == 10)
         break;
      player[pos + 1] = player[pos];
      player[pos] = tmp;
   }

   int int_tmp;
   int byte;
   char sm[100];
   char tmp2[1];
   gotoxy(10,37);
   printf("=========ranking=========\n");
   for (int i = 0; i < index; i++)
   {
      int_tmp = player[i].score;
      byte = 0;
      while ((int_tmp /= 10) != 0)
      {
         byte++;
      }
      byte++;
      sprintf(sm, "%d", player[i].score);
      gotoxy(10,38+i);
      printf("%d)name : %c%c%c, score : %s\n", i + 1, player[i].name[0], player[i].name[1], player[i].name[2],sm);
      write(fd, player[i].name, sizeof(char) * 3);
      write(fd, sm, sizeof(char) * byte);
      tmp2[0] = '/';
      write(fd, tmp2, sizeof(char));
   }
   return 0;
}
