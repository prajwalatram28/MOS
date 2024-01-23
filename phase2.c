#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Initialize 
char M[300][4];
char IR[5];
char R[4];
int IC;
int C;
int SI;

struct PCB
{
    char JID[4];
    int TTL;
    int TLL;
};

struct PCB info;
int flag = 0;
int currentLine;
int TTC;
int LLC;
int PI;
int TI;

int PTR;

int generated_number[100];
int generated_no_index;
int Program_card_couter;
int count = 0;

int key[30];
int value[30];
int key_index;

int gen_address;
int message;
char str[100];

int T;


// void print_status()
// {
    
//     printf("\nPI IS %d\n", PI);
//     printf("TI IS %d\n", TI);
//     printf("TTC IS %d\n", TTC);
//     printf("LLC IS %d\n", LLC);
//     printf("Ic IS %d\n", IC);
//     printf("T IS %d\n", T);
//     printf("IR IS %s\n", IR);
  
// }

// Random no genration
int Allocate()
{

    srand(time(NULL));
    int address;
    int l = 0;
    while (1)
    {
        address = rand() % 30;
        for (int i = 0; i <= generated_no_index; i++)
        {
            if (generated_number[i] == address)
            {
                l = 1;
                break;
            }
        }
        if (l == 0)
        {
            break;
        }
        else
        {
            l = 0;
        }
    }

    generated_no_index++;
    generated_number[generated_no_index] = address;

    return address;
}
// print karuga no generated
void print_generated_no()
{
    printf("\n\n");
    for (int i = 0; i <= generated_no_index; i++)
    {
        printf("%d ", generated_number[i]);
    }
    printf("\n\n");
}

// Memory block print
void print_memory_block()
{
    printf("Memory block is:\n\n");
    printf("\n\n");
    int i, j;
    for (i = 0; i < 300; i++)
    {
        if (i >= 100){
            printf("M[%d]  ", i);
    
        }

            
        else if (i >= 10 && i < 100){
            printf("M[%d]   ", i);
           
        }
        
        else{
            printf("M[%d]    ", i);
            }
        for (j = 0; j < 4; j++)
        {
            printf("%c", M[i][j]);
        }

        printf("\n");
    }
}

void print_instruction_register()
{
    // print the instruction register IR ki value
    int i;
    for (i = 0; i < 4; i++)
    {
        printf("%c", IR[i]);
    }
    printf("\n");
}


void print_general_purpose_register()
{ // print the R register ki value
    int i;
    for (i = 0; i < 4; i++)
    {
        printf("%c", R[i]);
    }
    printf("\n");
}

int Address_Map(int IC)
{
    if (IC % 10 == 0 && IC != 0)
    {
        count++;
    }
    int address = (M[PTR + count][2] - '0') * 10 + (M[PTR + count][3] - '0');

    address = address * 10 + IC % 10;

    return address;
}

int map(int add)
{
    // IR ki value ko map karna hai
    if (IR[0] == 'H' && IR[1] == ' ' && IR[2] == ' ' && IR[3] == ' ')
        return;

    if (IR[0] == 'B' && IR[1] == 'T')
        return;

    if (IR[2] - '0' < 0 || IR[2] - '0' > 9 || IR[3] - '0' < 0 || IR[3] - '0' > 9)
    {
        PI = 2;
        MOS();
        return;
    }

    for (int i = 0; i <= key_index; i++)
    {
        if (key[i] == (add / 10) * 10)
        {
            return (value[i]) * 10 + add % 10;
        }
    }

    {
        PI = 3;
        MOS();

        return;
    }
}

void HALT(int msg)
{
    // Yeh terminate karega
    T = 1;

    FILE *fp = NULL;
    fp = fopen("output_phase2.txt", "a");
    char id[10];
    fprintf(fp, "\nJob ID: %s\n", info.JID);

    switch (msg)
    {
    case 0:
        fputs("  No Error\n", fp);
        break;
    case 1:
        fputs("  OUT OF DATA \n", fp);
        break;
    case 2:
        fputs("   LINE LIMIT EXCEEDED \n", fp);
        break;
    case 3:
        fputs("  TIME LIMIT EXCEEDED \n", fp);
        break;
    case 4:
        fputs("   OPERATION CODE ERROR \n", fp);
        break;
    case 5:
        fputs("  OPERAND ERROR  \n", fp);
        break;
    case 6:
        fputs("  INVALID PAGE FAULT \n", fp);
        break;
    case 7:
        fputs("   TIME LIMIT EXCEEDED  And OPERATION CODE ERROR\n", fp);
        break;
    case 8:
        fputs("  TIME LIMIT EXCEEDED And OPERAND ERROR \n", fp);
        break;
    }
    fputs("IC : ", fp);
    fprintf(fp, "%d\n", IC);
    fputs("IR : ", fp);
    fputs(IR, fp);
    fputs("\nTTC : ", fp);
    fprintf(fp, "%d\n", TTC);
    fprintf(fp, "TTL : %d\n", info.TTL);
    fputs("LLC : ", fp);
    fprintf(fp, "%d\n", LLC);
    fprintf(fp, "TLL : %d\n\n", info.TLL);
}

void print_map()
{
    
    for (int i = 0; i <= key_index; i++)
    {
        printf("%d  %d\n", key[i], value[i]);
    }
    printf("\n\n");
}

void Load()
{
    // Load karega
    int i = 0;
    FILE *fp = NULL;
    char ch;
    fp = fopen("input_phase2.txt", "r");

    while (fgets(str, 41, fp) != NULL)
    {
       
        if (str[0] == '$' && str[1] == 'A' && str[2] == 'M' && str[3] == 'J')
        {
            PTR = Allocate() * 10;
            info.JID[0] = str[4];
            info.JID[1] = str[5];
            info.JID[2] = str[6];
            info.JID[3] = str[7];

            info.TTL = (str[8] - '0') * 1000 + (str[9] - '0') * 100 + (str[10] - '0') * 10 + (str[11] - '0');
            info.TLL = (str[12] - '0') * 1000 + (str[13] - '0') * 100 + (str[14] - '0') * 10 + (str[15] - '0');

            for (int i = PTR; i < PTR + 10; i++)
            {
                M[i][0] = '0';

                M[i][2] = '*';
                M[i][3] = '*';
            }

            printf("AMJ  found!!\n");
            printf("Job Id  is %s\n", info.JID);
            printf("Total Time Limit is %d\n", info.TTL);
            printf("Total Line Limit is %d\n\n\n", info.TLL);
            printf("Page table Register is %d\n", PTR);
       

            int col = 0;
            int temp, temp1;

            while (1)
            {

                fgets(str, 100, fp);
                if ((str[0] == '$' && str[1] == 'D' && str[2] == 'T' && str[3] == 'A'))
                    break;
                temp = Allocate();
                temp1 = temp * 10;
                //temp mein adreess haii
                M[PTR + Program_card_couter][0] = '1';
                M[PTR + Program_card_couter][3] = temp % 10 + '0';
                M[PTR + Program_card_couter][2] = temp / 10 + '0';
                Program_card_couter++;

                col = 0;
                for (int i = 0; i < strlen(str) - 1; i++)
                {
                    if (str[i] == ' ')
                        break;
                    M[temp1][col] = str[i];
                    col++;
                    if (col == 4)
                    {
                        temp1++;
                        col = 0;
                    }
                }
            }

            currentLine = ftell(fp);

        
        }
        if ((str[0] == '$' && str[1] == 'D' && str[2] == 'T' && str[3] == 'A'))
        {

            EXECUTE();
        }

        if ((str[0] == '$' && str[1] == 'E' && str[2] == 'N' && str[3] == 'D'))
        {
            i++;

            print_memory_block();
           

            printf("\n\n\nEND Job. %d\n\n\n", i);
            getch();
            printf("%s\n\n", str);

            if (message == 1)
            {
                fseek(fp, ftell(fp) + 2, SEEK_SET);
                printf("position is %d\n", ftell(fp));
            }

            
            INIT();
        }
    }
}

void INIT()
{
    // Initialize karega memory 300
    for (int i = 0; i < 300; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = ' ';
        }
    }
    for (int i = 0; i < 4; i++)
    {
        IR[i] = ' ';
        R[i] = ' ';
    }
    IC = 0;
    SI = 0;
    C = 0;
    T = 0;
    currentLine = 0;

    info.TTL = 0;
    info.TLL = 0;
    TTC = 0;
    LLC = 0;
    PI = 0;
    TI = 0;
    message = 0;

    generated_no_index = -1;
    Program_card_couter = 0;
    key_index = -1;
    count = 0;
}

void EXECUTE()
{
    IC = 0;
    EXECUTE_USER_PROGRAM();
}

void EXECUTE_USER_PROGRAM()
{
    while (1)
    {
        int addr = Address_Map(IC);
        for (int i = 0; i < 4; i++)
        {

            IR[i] = M[addr][i];
        }
        IC++;

        simulation();
        gen_address = map((IR[2] - '0') * 10 + (IR[3] - '0'));

        if (T == 1)
        {

            break;
        }

        if (!((IR[0] == 'G' && IR[1] == 'D') || (IR[0] == 'P' && IR[1] == 'D') || (IR[0] == 'H' && IR[1] == ' ') || (IR[0] == 'L' && IR[1] == 'R') || (IR[0] == 'S' && IR[1] == 'R') || (IR[0] == 'C' && IR[1] == 'R') || (IR[0] == 'B' && IR[1] == 'T')))
        {
            PI = 1;

            MOS();
            break;
        }

        if (TI == 2)
        {

            MOS();
            break;
        }

        else if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS();

            if (T == 1)
                break;
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();

            if (T == 1)
                break;
        }
        else if (IR[0] == 'H' && IR[1] == ' ')
        {
            SI = 3;
            MOS();
            break;
        }
        else if (IR[0] == 'L' && IR[1] == 'R')
        {
            LR();
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            SR();
        }
        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            BT();
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            CR();
        }
    }
}

void simulation()
{
    TTC++;
    if (TTC > info.TTL)
    {
        TI = 2;
    }
}

void MOS()
{

    if (TI == 0)
    {

        if (SI == 1)
        {
            GD();
        }
        else if (SI == 2)
        {
            PD();
        }
        else if (SI == 3)
        {
            H();
        }
        else if (PI == 1)
        {
            HALT(4);
        }
        else if (PI == 2)
        {
            HALT(5);
        }
        else if (PI == 3)
        {

            if ((IR[0] == 'G' && IR[1] == 'D') || (IR[0] == 'S' && IR[1] == 'R'))
            {

                key_index++;
                key[key_index] = (IR[2] - '0') * 10 + (IR[3] - '0');
                value[key_index] = Allocate();
                int temp = value[key_index];

                M[PTR + Program_card_couter][0] = '1';
                M[PTR + Program_card_couter][3] = temp % 10 + '0';
                M[PTR + Program_card_couter][2] = temp / 10 + '0';
                Program_card_couter++;
                PI = 0;
                return value[key_index] * 10;
            }
            
            else

            {

                HALT(6);
            }
        }
    }

    else if (TI == 2)
    {

        if (PI == 0)
        {

            if (IR[0] == 'P' && IR[1] == 'D')
            {
                PD();
            }
            else if (IR[0] == 'H' && IR[1] == ' ')
            {
                H();
                return;
            }
            HALT(3);
        }
       
        
        else if (PI == 1)
        {
            HALT(7);
        }
        else if (PI == 2)
        {
            HALT(8);
        }
        else if (PI == 3)
        {
            HALT(3);
        }
    }
}

void GD()
{

    int row = map((IR[2] - '0') * 10 + (IR[3] - '0'));

    int col = 0;

    FILE *fp = NULL;
    fp = fopen("input_phase2.txt", "r");

    fseek(fp, currentLine, SEEK_SET);
    fgets(str, 100, fp);
    if ((str[0] == '$' && str[1] == 'E' && str[2] == 'N' && str[3] == 'D'))
    {

        message = 1;
        HALT(1);
        return;
    }
    for (int i = 0; i < strlen(str) - 1; i++)
    {
        M[row][col] = str[i];
        col++;
        if (col == 4)
        {
            row++;
            col = 0;
        }
    }
    currentLine = ftell(fp);
    SI = 0;
}

void PD()
{
    FILE *fp = NULL;
    LLC++;
    if (LLC > info.TLL)
    {
        HALT(2);

        return;
    }
    fp = fopen("output_phase2.txt", "a");


    if (flag == 0)
        flag++;
    else
        fprintf(fp, "\n");

    int row = map((IR[2] - '0') * 10 + (IR[3] - '0'));

    int col = 0;
    for (int i = row; i < row + 10; i++)
    {
        for (int j = 0; j < 4; j++)
        {

            fputc(M[i][j], fp);
        }
    }
    SI = 0;
}
void H()
{

    HALT(0);
}
// void H()
// {

//     Terminate(0);
// }

void LR()
{
    int row = map((IR[2] - '0') * 10 + (IR[3] - '0'));
    for (int i = 0; i < 4; i++)
    {
        R[i] = M[row][i];
    }
}

void SR()
{
    int row = map((IR[2] - '0') * 10 + (IR[3] - '0'));
    for (int i = 0; i < 4; i++)
    {
        M[row][i] = R[i];
    }
}
void BT()
{
    if (C == 1)
    {
        IC = (IR[2] - '0') * 10 + (IR[3] - '0');
        C = 0;
    }
}

void CR()
{
    int row = map((IR[2] - '0') * 10 + (IR[3] - '0'));

    for (int i = 0; i < 4; i++)
    {
        if (R[i] != M[row][i])
        {
            C = 0;
            break;
        }
        else
        {
            C = 1;
        }
    }
}


int main()
{

    printf("\n\n ************************************************************************************************************************\n\n\n\n");

    printf("\t\t\t\t\t\t|  Group 9 B2  |\n\n\n");

    printf("Prajwal Atram      \n");
    printf("Hitashri Patil     \n");
    printf("Nupur Shinde    \n");
    printf("Vishal Singh     \n");
    printf("Sameer  Meshram    \n\n");

    printf("Phase 2 Implementation \n\n");

    FILE *f = NULL;
    char ch;
    f = fopen("input_phase2.txt", "r");
    if (f == NULL)
    {
        printf("not found!!");
        exit(1);
    }
    while ((ch = fgetc(f)) != EOF)
    {
        //Sab kuch read karuga input file  mein se 
        // printf("%c", ch);
    }
    fclose(f);

    printf("\n\n\nEnter any key to continue\n\n");
    getch();
    
   


    INIT();

    Load();

    return 0;
}