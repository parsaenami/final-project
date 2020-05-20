#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include <winnt.h>


int temp, level_number, level_cnt, fcount, z = 3;
int countr2 = 1;
double score1, score2, score3, sum_score, word_score, levels_score;
clock_t t1, t2, p1, p2, total_time, paused_time, playing_time, playing_time_all;


struct player
{
    char p_name[50];
    int p_level;
    double p_score;
};
struct player person;
struct node
{
    char words[20];
    int number_of_word;
    struct node * next;
};
int level_info ()
{
    int i;
    FILE * fl;
    for (i = 1; fl != NULL; i++)
    {
        char FileName[100] = "level-";
        char FileFormat[] = ".txt";
        char FileNumber[10];
        sprintf(FileNumber, "%d", i);
        strcat(FileName, FileNumber);
        strcat(FileName, FileFormat);
        fl = fopen (FileName, "r");
        if (fl != NULL)
            fcount++;
        fclose (fl);
//        printf("file = %s\t\tcount = %d\n\n", str1, fcount);
    }
    return fcount;
}
void name(char s[50])
{
    strcpy(person.p_name, s);
//    FILE * fp = fopen("enterance.txt", "ab");
//    fwrite(s, sizeof(char), 50, fp);
}
void gotoxy (int x,int y)
{
    COORD coord = {x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void SetColor (int ForgC)
{
    WORD wColor;
    //This handle is needed to get the current background attribute
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    //csbi is used for wAttributes word
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
    {
        //To mask out all but the background attribute, and to add the color
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}
void add_end (struct node *list, char word[100])
{
    struct node *current = list;
    for (; current->next != NULL; current = current->next);
    struct node *new_node = (struct node *)malloc (sizeof (struct node));
    strcpy(new_node->words, word);
    new_node->next = NULL;
    current->next = new_node;
}
void delete_node (struct node** header, int num)
{
    struct node* temp;
    struct node* current = (*header);
    if( current != NULL && 0 == num)
    {
        temp = current;
        current = current->next;
        free (temp);
        (*header) = current;
        return ;
    }
    int cnt = 0;
    while(cnt < num - 1)
    {
        cnt++;
        current = current->next;
    }
    temp = current->next;
    current->next = temp->next;
    free (temp);
    return;
}
struct node * level (int x)
{
    FILE * fl;
    char FileName[100] = "level-";
    char FileFormat[] = ".txt";
    char FileNumber[10];
    sprintf(FileNumber, "%d", x);
    strcat(FileName, FileNumber);
    strcat(FileName, FileFormat);
    fl = fopen(FileName, "r");
    if(fl == NULL)
        printf("Not opened\n");
    struct node *list = (struct node *)malloc(sizeof(struct node)), * tmp;
    tmp = list;
    tmp->next = NULL;
    list->number_of_word = countr2;
    countr2++;
    char c;
    char word[100];
    int i, counter = 0;
    while (fscanf (fl, " %s", word) == 1)
    {
        //  printf("%s\n",word);
        counter++;
        add_end (tmp,word);
    }
    temp = counter;
    //printf("counter = %d\n",temp);
    Sleep (1000);
//       printf("counter is = %d", counter);
    fclose (fl);
    return list;
}
int random (int a, int b)
{
    int c;
    time_t t = time(NULL);
    srand (t);
    c = (rand() % (a - b + 1)) + b;
    return c;
}
int uppercase3 (char str[100], int i, char get_ch)
{
    int j;
    if (str[i] == get_ch)
    {
        str[i] = str[i] - 32;
        SetColor (2);
        for (j = 0; j <= i; j++)
            printf("%c", str[j]);
        SetColor (0);
        for (j = i + 1; j <= strlen (str); j++)
            printf("%c", str[j]);
        return 1;
    }
    if (str[i] != get_ch)
    {
        SetColor (2);
        for (j = 0; j < i; j++)
            printf ("%c", str[j]);
        SetColor (12);
        printf ("%c",str[i]);
        SetColor (0);
        for (j = i + 1; j <= strlen(str); j++)
            printf ("%c", str[j]);
        return 0;
    }
}
double score_method (int score)
{
    switch (score)
    {
    case 1:
        return score1;
    case 2:
        return score2;
    case 3:
        return score3;
    }
}
void save ()
{
    char player_name[50];
    int f = 0;
    struct player tmp;
    person.p_level = level_number;
    person.p_score = sum_score / level_cnt;
    FILE * fp = fopen("enterance.bin", "rb+");
    if (fp == NULL)
    {
        fp = fopen("enterance.bin", "w+");
        fwrite(&person, 1, sizeof(person), fp);
    }
    else
    {
        while (!feof(fp))
        {
            fread(&tmp, 1, sizeof(person), fp);
            if (strcmp(person.p_name, tmp.p_name) == 0)
            {
                fseek(fp, -1 * sizeof(person), SEEK_CUR);
                fwrite(&person, 1, sizeof(person), fp);
                f = 1;
                break;
            }
        }
        if (f == 0)
            fwrite(&person, 1, sizeof(person), fp);
    }
    fclose(fp);
}
int pause ()
{
    char k;
    clock_t p1, p2, paused_time;
    gotoxy (25, 12);
    printf ("Pause!");
    p1 = clock();
    while ('R' != getch())
        k = getch();
    gotoxy (25, 12);
    printf ("Resume!");
    p2 = clock();
    return p2 - p1;
}
void quit ()
{
    int yn;
    printf ("You Have Been Exited The Game. Do You Want To Save Your Data?\n\t\t\t[1]Yes\n\t\t\t[2]No\n");
    gotoxy (74, 17);
    scanf ("%d", &yn);
    switch (yn)
    {
    case 1:
        save();
        z++;
        break;
    case 2:
        break;
    default:
        break;
    }
    gotoxy (12, 20);
    printf ("thanks for your playing!\n");
    gotoxy (12, 22);
    printf ("your record is here :\n\t\t\ttotal score = %lf\n\n\n", sum_score / level_cnt);
    exit (1);
}
void read_paste (struct node **list, int score)
{
    struct node *current = *list;
    int i;
    double false_m = 0, true_m = 0;
    int count = temp;
    for (; 0 < count; count--)
    {
        current = *list;
        //printf("before randomize count is = %d\n", count);
        int c = random(count, 1);
        while (count == temp && c == 0)
            c = random(count - 1, 0);
        for (i = 0; i < c; current = current->next, i++);
        t1 = clock();
        gotoxy (24, 0);                                      /**junk maybe...*/
        printf ("%s\n", current->words);
        char k;
        int j, velin = 0, x;
        for (j = 0; j<strlen(current->words); j++)
        {
            k = getch();
            switch (k)
            {
            case 'P':
                paused_time += pause ();
                false_m--;
                break;
            case 'Q':
                false_m--;
                gotoxy (12, 17);
                quit ();
                break;
            }
            system ("cls");
//            for(x=1; x <= velin; x++)
//                gotoxy(24, x);
//            velin++;
            gotoxy (24, 0);                                      /**junk maybe...*/
            if (!uppercase3(current->words, j, k))
            {
                j--;
                false_m++;
                //printf("falseeeeeeeeeeee = %d\n", false_m);
            }
            else
            {
                true_m++;
                //printf("truuuuuuuuuuuuuu = %d\n", true_m);
                //printf("now counter is = %d \nprev temp is = %d\n", count, temp);
            }
            t2 = clock();
            total_time = t2 - t1;
            playing_time = total_time - paused_time;
            playing_time_all += playing_time;
            score1 = (true_m * 3 - false_m) / playing_time * 1000;
            score2 = true_m * 3 / playing_time * 1000;
            score3 = (true_m - false_m) / playing_time * 1000;
            word_score = score_method (score);
//            gotoxy(20, 1);
            //printf("playing_time = %d", playing_time / 1000);
//            gotoxy(20, 2);
            false_m = true_m = 0;
            sum_score += word_score;
        }
        system ("cls");
        delete_node (list, c);
    }
}
int check ()
{
    FILE * fp = fopen ("enterance.bin", "rb+");
    if (fp == NULL)
        return 0;
    struct player tmp;
    while (!feof (fp))
    {
        fread (&tmp, 1, sizeof (person), fp);

        if (strcmp (person.p_name, tmp.p_name) == 0)
        {
//            printf("1 === %s\n2 === %s\n", person.p_name, tmp.p_name);
//            	printf("Your Last Score Was %lf\n", tmp.p_score);
//            	printf("Your Last Level Was %d\n", tmp.p_level);
            return tmp.p_level;
        }
    }
    return 0;
}
int top_ten()
{
    struct player temp[100];
    FILE *fs = fopen("enterance.bin","rb");
    int i = 0;
    while (!feof(fs))
    {
        fread (&temp[i], 1, sizeof (struct player ), fs);
        i++;
    }
    int d, f;
    for (d = 0; d < i; d++)
    {
        for (f = 0; f < i; f++)
        {
            if (temp[d].p_score < temp[f].p_score)
            {
                struct player t;
                t = temp[d];
                temp[d] = temp[f];
                temp[f] = t;
            }
        }
    }
    int k;
    for (k = i; k >= 0; k--)
    {
        printf("%s     %lf\n", temp[k].p_name, temp[k].p_score);
    }
}
int main ()
{
    void quit ();
    int score, menu;
    char player_name[50];
    gotoxy(24, 0);
    SetColor(13);
    printf("Hello!");
    SetColor(5);
    printf ("\n\n\t\tPlease Enter Your Name : ");
    SetColor(10);
    scanf ("%s", player_name);
    name(player_name);
    SetColor(0);
    gotoxy(15, 4);
    printf ("------------------------");
    gotoxy(22, 5);
    printf ("Well Done!");
    gotoxy(15, 7);
    SetColor(13);
    printf ("Now What Do You Want To Do ? : ");
    SetColor(5);
    printf ("\n\n\t\t[1]Start a New Game\n\n\t\t[2]Start Saved Game\n\n\t\t[3]Exit\t\t[4]Show Top Players\n");
    SetColor(10);
    int k = 0;
    k = check();
    gotoxy (46, 7);
    scanf ("%d", &menu);
    gotoxy (12, 16);
    if (menu == 1)
    {
        SetColor(13);
        printf ("Please Enter Level Between 1 to %d : ", level_info ());
        SetColor(10);
        scanf ("%d", &level_number);
        while (level_number < 1 || level_number > level_info ())
        {
            SetColor(13);
            printf ("Sorry! Wrong Level Number!\nPlease Enter Another Number : ");
            SetColor(10);
            scanf ("%d", &level_number);
        }
    }
    else if (menu == 2 && k != 0)
    {
        level_number = k;
        SetColor(13);
        printf ("Starting From Level %d\n", k);
    }
    else if (menu == 2 && k == 0)
    {
        printf ("You Had No Save:(\n");
        return 0;
    }
    else if (menu == 3)
        exit (1);
    else if ( menu == 4)
    {
        SetColor(0);
        top_ten();
        exit (1);
    }
    else
        printf ("Wrong Choice!");
    SetColor(0);
    gotoxy(15, 18);
    printf ("------------------------");
    gotoxy(25, 19);
    printf ("OK!\n\n");
    SetColor(13);
    gotoxy(15, 21);
    printf ("We Have Some Scoring Methods:\n");
    SetColor(5);
    printf ("\n\t\t[1]Ordinary Method\n\n\t\t[2]Lenient Method\n\n\t\t[3]Strict Method\n");
    SetColor(13);
    printf ("\n\tThese Are Our Methods...Choose Your Ideal One! : \n");
    SetColor(10);
    gotoxy(57, 29);
    scanf ("%d", &score);
    SetColor(0);
    system ("cls");

    for (; level_number <= level_info(); level_number++)
    {
        struct node * t = level(level_number);
        level_cnt++;
        read_paste(&t, score);
        system("cls");
        gotoxy(12, 10);
        printf ("Your Record Till Now Is Here :\n\t\tTotal Score = %lf\n", sum_score / level_cnt);
        gotoxy (12, 15);
        if (level_number < level_info())
        {
            printf ("Do You Want To Keep On Playing?\n\t\t\t[1]Yes\n\t\t\t[2]No\n");
            int x;
            gotoxy (44, 15);
            scanf ("%d", &x);
            system ("cls");
            if (x == 2)
            {
                gotoxy (12, 15);
                printf ("The Game Has Finished!\n");
                gotoxy (12, 17);
                quit();
            }
        }
        else if (level_number == level_info())
        {
            printf ("End Of The Game!\n");
            gotoxy (12, 17);
            quit();
        }
    }
    return 0;
}
