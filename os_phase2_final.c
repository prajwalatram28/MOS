//os phase 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int flag = 0;
int currentLine ;


char Memory_block [300][4];
char Instruction_register [4];
char General_purpose_register [4];
int Instruction_counter;
int Toggle_register;
int System_interrupt;

struct PCB{
    char JID[4];
    int TTL;
    int TLL; 
};

struct PCB info;

int TTC;
int TLC;
int PI;
int TI;

int PTR;

int generated_no[100];
int generated_no_index;
int Program_card_couter;
int count=0;

int key[30];
int value[30];
int key_index;

int gen_address;
int message;
char str[100];


void print_status(){
    printf("\nPI IS %d\n",PI);
    printf("TI IS %d\n",TI);
    printf("TTC IS %d\n",TTC);
    printf("TLC IS %d\n",TLC);
    printf("Ic IS %d\n",Instruction_counter);
    printf("IR IS %s\n",Instruction_register);
}

//Allocate function to generate a random memory block
int Allocate(){
    srand(time(NULL));
    int address;
    int l=0;
    while(1){
        address = rand()%30;
        for(int i=0;i<=generated_no_index;i++){
        if(generated_no[i] == address){
           l=1;
              break;
        }
       
        }
        if(l==0){
            break;
        }
        else{
            l=0;
        }


    }
    
    
    generated_no_index++;
    generated_no[generated_no_index] = address;
    

    return address;
}

void print_generated_no(){
    printf("\n\n");
    for(int i=0;i<=generated_no_index;i++){
        printf("%d ",generated_no[i]);
    }
    printf("\n\n");
}

//print the memory block
void print_memory_block()
{
    printf("Memory block is:\n\n");
    printf("\n\n");
    int i,j;
    for(i=0;i<300;i++)
    {   if(i>=100) printf("%d  ",i);
        else if(i>=10 && i<100) printf("%d   ",i);
        else printf("%d    ",i);
        for(j=0;j<4;j++)
        {
            printf("|%c",Memory_block[i][j]);
        }
        
        printf("|\n");
        
    
    }
}
//print the instruction register
void print_instruction_register()
{
    int i;
    for(i=0;i<4;i++)
    {
        printf("%c",Instruction_register[i]);
    }
    printf("\n");
}

//print the general purpose register
void print_general_purpose_register()
{
    int i;
    for(i=0;i<4;i++)
    {
        printf("%c",General_purpose_register[i]);
    }
    printf("\n");
}


int Address_Map(int IC ){
        if(IC%10 == 0 && IC!=0){
            count++;
        }
        int address=(Memory_block[PTR+count][2]-'0')*10+(Memory_block[PTR+count][3]-'0');
        
        address=address*10+IC%10;
        
        return address;
    
        
    }
    
int map(int add){
    if(Instruction_register[2]-'0'<0 || Instruction_register[2]-'0'>9 || Instruction_register[3]-'0'<0 || Instruction_register[3]-'0'>9){
        PI=2;
        message=5;
        return -1;
    } 
    if(Instruction_register[0]=='B' && Instruction_register[1]=='T'){
        return;
    }
    for(int i=0;i<=key_index;i++){
        if(key[i]==(add/10)*10){
            return (value[i])*10+add%10;
        }
    }
    if((Instruction_register[0]=='G' && Instruction_register[1]=='D') || (Instruction_register[0]=='S' && Instruction_register[1]=='R') ){
     
    key_index++;
    key[key_index]=add;
    value[key_index]=Allocate();
    int temp = value[key_index];
    // printf("Allocated memory block is %d for %d operand\n\n\n",value[key_index] ,key[key_index]);
    Memory_block[PTR+Program_card_couter][0]='1';
    Memory_block[PTR+Program_card_couter][3]=temp%10+'0';
    Memory_block[PTR+Program_card_couter][2]=temp/10+'0';
    Program_card_couter++;
    return value[key_index]*10;
    }
    else{
        PI=3;
        message=6;
        return -1;

    }
}

void print_end_of_job(int msg){
    
    FILE *fp=NULL;
    fp=fopen("o2.txt","a");
    char id[10];
    fprintf(fp,"\nJob ID: %s\n",info.JID);
    
    
    switch (msg)
    {
    case 0:
        fputs("  Job terminated normally no error\n",fp);
        break;
    case 1:
        fputs("  Data out of range \n",fp);
        break;
    case 2:
        fputs("  Line limit exceeded error \n",fp);
        break;
    case 3:
        fputs("  Time limit exceeded error \n",fp);
        break;
    case 4:
        fputs("  Invalid opcode error \n",fp);
        break;
    case 5:
        fputs("  Invalid operand error \n",fp);
        break;
    case 6:
        fputs("  Invalid page fault error \n",fp);
        break;
    case 7:
        fputs("  Time limit exceeded and invalid opcode \n",fp);
        break;
    case 8:
        fputs("  Time limit exceeded and invalid operand \n",fp);
        break;
    case 9:
        fputs("  Time limit exceeded and invalid page fault \n",fp);
        break;
    
    }
    fputs("IC is ",fp);
    fprintf(fp,"%d\n",Instruction_counter);
    fputs("IR has ",fp);
    fputs(Instruction_register,fp);
    fputs("\nTTC is ",fp);
    fprintf(fp,"%d\n",TTC);
    fputs("TLC is ",fp);
    fprintf(fp,"%d\n\n",TLC);



}

void print_map(){
    // printf("key index is %d \n\n",key_index);
    // getchar();
    for(int i=0;i<=key_index;i++){
        printf("%d  %d\n",key[i],value[i]);
    }
    printf("\n\n");
}




void Load(){
int i=0;
FILE *fp=NULL;
char ch;
fp=fopen("i2.txt","r");


while (fgets(str, 41, fp) != NULL)
{
    // printf("%s",str);
    // printf("position is %d\n",ftell(fp));
    if (str[0]=='$' && str[1]=='A' && str[2]=='M' && str[3]=='J')
    {
        PTR=Allocate()*10;
        info.JID[0]=str[4];
        info.JID[1]=str[5];
        info.JID[2]=str[6];
        info.JID[3]=str[7];

        info.TTL=(str[8]-'0')*1000+(str[9]-'0')*100+(str[10]-'0')*10+(str[11]-'0');
        info.TLL=(str[12]-'0')*1000+(str[13]-'0')*100+(str[14]-'0')*10+(str[15]-'0');

    for(int i=PTR;i<PTR+10;i++){
        Memory_block[i][0]='0';

        Memory_block[i][2]='*';
        Memory_block[i][3]='*';
    }
        

        printf("AMJ instruction found\n");
        printf("JID is %s\n",info.JID);
        printf("TTL is %d\n",info.TTL);
        printf("TLL is %d\n\n\n",info.TLL);
        printf("PTR is %d\n",PTR);
        getchar();
        // print_memory_block();
        // getchar();
        
        
        int col=0;
        int temp,temp1;

        while(1)
        {
           
        // print_memory_block();
        // print_generated_no();
        // getchar();    
            
        fgets(str, 100, fp);
        if((str[0]=='$' && str[1]=='D' && str[2]=='T' && str[3]=='A')) break;
        temp=Allocate();
        temp1=temp*10;
        Memory_block[PTR+Program_card_couter][0]='1';
        Memory_block[PTR+Program_card_couter][3]=temp%10+'0';
        Memory_block[PTR+Program_card_couter][2]=temp/10+'0';
        Program_card_couter++;
 
        
     
       
        
        col=0;
       for(int i=0;i<strlen(str)-1;i++){
        if (str[i]==' ') break;
        Memory_block[temp1][col]=str[i];
        col++;
        if(col==4){
            temp1++;
            col=0;
        }
        


       }
       
       
        }
        
        currentLine =ftell(fp);

    // printf("loading is completed\n");
    

    
    // printf("\nenter to start execution:\n");
    // getch();
        
    }
    if((str[0]=='$' && str[1]=='D' && str[2]=='T' && str[3]=='A')){


    START_EXECUTION();


    }


    if((str[0]=='$' && str[1]=='E' && str[2]=='N' && str[3]=='D')){
        i++;
        
        print_memory_block();

        printf("\n\n\nEND OF JOB no. %d\n\n\n",i);
        printf("%s\n\n",str);
        
        if(message==1){
            fseek(fp,ftell(fp)+2,SEEK_SET);
            printf("position is %d\n",ftell(fp));
        }
        
        
        print_end_of_job(message);
        
        printf("enter to load another job:\n");
        getchar();
        INIT();
    }
    }

    
}




void INIT(){
for(int i=0;i<300;i++){
for(int j=0;j<4;j++){
Memory_block[i][j]=' ';
}
}
for(int i=0;i<4;i++){
Instruction_register[i]=' ';
General_purpose_register[i]=' ';
}
Instruction_counter=0;
System_interrupt=0;
Toggle_register=0;
currentLine=0;

info.TTL=0;
info.TLL=0;
TTC=0;
TLC=0;
PI=0;
TI=0;
message=-1;

generated_no_index=-1;
Program_card_couter=0;
key_index=-1;
count=0;

}




void START_EXECUTION(){
    Instruction_counter=0;
    EXECUTE_USER_PROGRAM();
}



void EXECUTE_USER_PROGRAM(){
    while(1){
    int addr=Address_Map(Instruction_counter);
    for(int i=0;i<4;i++){
        
        Instruction_register[i]=Memory_block[addr][i];
    }
    Instruction_counter++;
    // printf("\n\nInstruction register has:\n\n");
    
    
    gen_address=map((Instruction_register[2]-'0')*10+(Instruction_register[3]-'0'));
    simulation();
    // print_map();
    // print_status();
    // getchar();
    if(!((Instruction_register[0]=='G' && Instruction_register[1]=='D') || (Instruction_register[0]=='P' && Instruction_register[1]=='D' )|| (Instruction_register[0]=='H' && Instruction_register[1]==' ') || (Instruction_register[0]=='L' && Instruction_register[1]=='R') || (Instruction_register[0]=='S' && Instruction_register[1]=='R') || (Instruction_register[0]=='C' && Instruction_register[1]=='R') || (Instruction_register[0]=='B' && Instruction_register[1]=='T'))){
        PI=1;
        
    }
    if(TI==2){
        if(PI==1){
            message=7;
            break;
        }
       else if(PI==2){
            message=8;
            break;
        }
    else if(PI==3){
            message=9;
            break;
        }
    else if(Instruction_register[0]=='P' && Instruction_register[1]=='D'){
            PD();
            TLC++;
            message=3;
            break;
        }
        else{
            message=3;
            break;
        }
    }
    else if(TI==0){
        if(PI==1){
            message=4;
            break;
        }
         else if(PI==2){
                message=5;
                break;
          }
        else if(PI==3){
                message=6;
                break;
          }
    }
       
    
    if(Instruction_register[0]=='G' && Instruction_register[1]=='D'){
        System_interrupt=1;
        GD();
        if(message==1) break;
    }
    else if(Instruction_register[0]=='P' && Instruction_register[1]=='D'){
        System_interrupt=2;
        TLC++;
        if(TLC>info.TLL){
            message=2;
            break;
        }
        else{
            PD();
        }
        
    }
    else if(Instruction_register[0]=='H' && Instruction_register[1]==' '){
        System_interrupt=3;
        H();
        message=0;
        printf("execution is completed\n");
        break;
    }
    else if(Instruction_register[0]=='L' && Instruction_register[1]=='R'){
        LR();
    }
    else if(Instruction_register[0]=='S' && Instruction_register[1]=='R'){
        SR();
    }
    else if(Instruction_register[0]=='B' && Instruction_register[1]=='T'){
        BT();
    }
    else if(Instruction_register[0]=='C' && Instruction_register[1]=='R'){
        CR();
    }
    
    
    }
}


void simulation(){
    TTC++;
    if(TTC>info.TTL){
        TI=2;
        message=3;
    }
}



// void MOS(){
//     if(System_interrupt==1){
//         GD();
//     }
//     else if(System_interrupt==2){
//         PD();
//     }
//     else if(System_interrupt==3){
//         H();
//     }
// }

void GD(){
    // printf("press enter to check memory\n");
    // getch();
   int row = map((Instruction_register[2]-'0')*10+(Instruction_register[3]-'0'));
//    printf("row is %d\n",row);
    int col = 0;
    // char str[100];
    FILE *fp=NULL;
    fp=fopen("i2.txt","r");

    fseek(fp,currentLine,SEEK_SET);
    fgets(str, 100, fp);
    if((str[0]=='$' && str[1]=='E' && str[2]=='N' && str[3]=='D')){
        printf("current line is %d %d\n",currentLine,ftell(fp));
        getchar();
        message=1;
        return;
    }
    for(int i=0;i<strlen(str)-1;i++){
        Memory_block[row][col]=str[i];
        col++;
        if(col==4){
            row++;
            col=0;
        }
    }
    currentLine =ftell(fp);
    // print_memory_block();

    
}

void PD(){
    FILE *fp=NULL;
    fp=fopen("o2.txt","a");
    if(flag==0) flag++;
    else fprintf(fp,"\n");
    // printf("press enter to write output\n");
    // getch();
    int row = map((Instruction_register[2]-'0')*10+(Instruction_register[3]-'0'));
    // printf("row is %d\n",row);
    // getchar();
    int col = 0;
    for(int i=row;i<row+10;i++){
        for(int j=0;j<4;j++){
            
            fputc(Memory_block[i][j],fp);

        }
        
        
    }
    
}
void H(){
    


    
}

void LR(){
    int row = map((Instruction_register[2]-'0')*10+(Instruction_register[3]-'0'));
    for(int i=0;i<4;i++){
        General_purpose_register[i]=Memory_block[row][i];
    }

    
    // printf("General purpose register has:\n\n");
    // getchar();
    // print_general_purpose_register();
}


void SR(){
    int row = map((Instruction_register[2]-'0')*10+(Instruction_register[3]-'0'));
    for(int i=0;i<4;i++){
        Memory_block[row][i]=General_purpose_register[i];
    }
    
    // printf("Memory block has:\n\n");
    // getchar();
    // print_memory_block();
}

void CR(){
    int row = map((Instruction_register[2]-'0')*10+(Instruction_register[3]-'0'));
    
    for(int i=0;i<4;i++){
        if(General_purpose_register[i]!=Memory_block[row][i]){
            Toggle_register=0;
            break;
            
        }
        else{
            Toggle_register=1;
        }
    }
    
}


void BT(){
    if(Toggle_register==1){
        Instruction_counter=(Instruction_register[2]-'0')*10+(Instruction_register[3]-'0');
        Toggle_register=0;
    }
}


//write main function
int main()
{
    //declare variables
    printf("\n\n######################################################################################## B1 Batch - Group 2 #####################################################################################\n\n\n\n"); 












    printf("\t\t\t\t\t\t\t\t\t\t\tAmey Chopde        13\n");
    printf("\t\t\t\t\t\t\t\t\t\t\tVishal Govindani   18\n");
    printf("\t\t\t\t\t\t\t\t\t\t\tKuhu Mukhopadhay   32\n");
    printf("\t\t\t\t\t\t\t\t\t\t\tTanmay Mutalik     33\n\n");
    printf("This program will show implementation of os phase 1\n");
    printf("The code written in input.txt is\n\n");
    //code to read from a file and print the contents on the screen




    FILE *f=NULL;
    char ch;
    f=fopen("i2.txt","r");
    if(f==NULL)
    {
        printf("File not found");
        exit(1);
    }
    while((ch=fgetc(f))!=EOF)
    {
        printf("%c",ch);
    }
fclose(f);

printf("\n\n\nEnter any key to continue\n\n");
getch();
    
// printf("\n\nInit is called first\n\n");
INIT();
// print_memory_block();
// printf("\n\nenter any key to continue\n\n");
// getch();
// printf("\n\nLoad is called next\n\n");
Load();

    return 0; 
}