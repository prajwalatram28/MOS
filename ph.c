#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int flag = 0;
int currentLine ;


char Memory_block [100][4];
char Instruction_register [4];
char General_purpose_register [4];
int Instruction_counter;
int Toggle_register;
int System_interrupt;







//print the memory block
void print_memory_block()
{
    printf("Memory block is:\n\n");
    printf("\n\n");
    int i,j;
    for(i=0;i<100;i++)
    {   if(i>=10) printf("%d  ",i);
        else printf("%d   ",i);
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



void Load(){
int i=0;
FILE *fp=NULL;
char ch;
fp=fopen("input.txt","r");
char str[100];
while (fgets(str, 41, fp) != NULL)
{
    if (str[0]=='$' && str[1]=='A' && str[2]=='M' && str[3]=='J')
    {
        // printf("AMJ instruction found\n");
        // getchar();
        int row=0;
        int col=0;
        
        while(1)
        {
           
            
            
        fgets(str, 100, fp);
        
     
       
        if((str[0]=='$' && str[1]=='D' && str[2]=='T' && str[3]=='A')) break;
        
       for(int i=0;i<strlen(str)-1;i++){
        if (str[i]==' ') break;
        Memory_block[row][col]=str[i];
        col++;
        if(col==4){
            row++;
            col=0;
        }
        


       }
       
       
        }
        
        currentLine =ftell(fp);

    // printf("loading is completed\n");
    

     
        
    }
    if((str[0]=='$' && str[1]=='D' && str[2]=='T' && str[3]=='A')){
START_EXECUTION();
    }
    if((str[0]=='$' && str[1]=='E' && str[2]=='N' && str[3]=='D')){
        i++;
        // print_memory_block();
        printf("\n\n\nEND OF JOB no. %d\n\n\n",i);
        printf("enter to load another job:\n");
        getchar();

        INIT();
    }
    }
    
    
}




void INIT(){
for(int i=0;i<100;i++){
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

}




void START_EXECUTION(){
    Instruction_counter=0;
    EXECUTE_USER_PROGRAM();
}



void EXECUTE_USER_PROGRAM(){
    while(1){
    for(int i=0;i<4;i++){
        Instruction_register[i]=Memory_block[Instruction_counter][i];
    }
    Instruction_counter++;
    // printf("\n\nInstruction register has:\n\n");
    // print_instruction_register();
    if(Instruction_register[0]=='G' && Instruction_register[1]=='D'){
        System_interrupt=1;
        MOS();
    }
    else if(Instruction_register[0]=='P' && Instruction_register[1]=='D'){
        System_interrupt=2;
        MOS();
    }
    else if(Instruction_register[0]=='H' && Instruction_register[1]==' '){
        System_interrupt=3;
        MOS();
        // printf("execution is completed\n");
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




void MOS(){
    if(System_interrupt==1){
        GD();
    }
    else if(System_interrupt==2){
        PD();
    }
    else if(System_interrupt==3){
        H();
    }
}

void GD(){
    
   int row = (Instruction_register[2]-'0')*10+(Instruction_register[3]-'0');
    int col = 0;
    char str[100];
    FILE *fp=NULL;
    fp=fopen("input.txt","r");

    fseek(fp,currentLine,SEEK_SET);
    fgets(str, 100, fp);
    for(int i=0;i<strlen(str)-1;i++){
        Memory_block[row][col]=str[i];
        col++;
        if(col==4){
            row++;
            col=0;
        }
    }
    currentLine =ftell(fp);
    

    
}

void PD(){
    FILE *fp=NULL;
    fp=fopen("output.txt","a");
    if(flag==0) flag++;
    else fprintf(fp,"\n");
    
    int row = (Instruction_register[2]-'0')*10+(Instruction_register[3]-'0');
    int col = 0;
    for(int i=row;i<row+10;i++){
        for(int j=0;j<4;j++){
            if(Memory_block[i][j]=='_') break;
            fputc(Memory_block[i][j],fp);

        }
        
        
    }
    
}
void H(){
    FILE *fp=NULL;
    fp=fopen("output.txt","a");
    fputc('\n',fp);
    fputc('\n',fp);


    
}

void LR(){
    int row = (Instruction_register[2]-'0')*10+(Instruction_register[3]-'0');
    for(int i=0;i<4;i++){
        General_purpose_register[i]=Memory_block[row][i];
    }

    
}


void SR(){
    int row = (Instruction_register[2]-'0')*10+(Instruction_register[3]-'0');
    for(int i=0;i<4;i++){
        Memory_block[row][i]=General_purpose_register[i];
    }
    
}
void CR(){
    int row = (Instruction_register[2]-'0')*10+(Instruction_register[3]-'0');
    
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
    f=fopen("input.txt","r");
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

    

INIT();


getch();

Load();

    return 0; 
}