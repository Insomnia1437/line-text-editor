#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lite.h"
#define LINELENGTH 1000// max length of a line
#define FNAMELENGTH 100
#define CMDLENGTH 102


static int currentlinenumber=1;

static int hasfilename=0;//boolean type to record filename exist or not
static int filechanged=0;//record status of file
static int filesaved=1;//record status of file
static int maxlinenumber=0;//record the max line number of the file.
Text head;//head is set to be global,representing the current linked list.

//create a text from the command line
Text createText()
{
    head = (Text)malloc(sizeof(Line));
    Line *curr;
    Line *temp;
    curr=temp=head;
    int lnumber=1;
    //printf("please input:\n");
    while(1)
    {
        char *current=(char*)malloc(sizeof(char)*LINELENGTH);
        memset(current,0,sizeof(char)*LINELENGTH);
        gets(current);
        if(*current == '.')
            break;
        curr = (Text)malloc(sizeof(Line));
        memset(curr,0,sizeof(Line));
        curr->data=current;
        curr->linenumber=lnumber++;
        maxlinenumber++;
        temp->next=curr;
        temp=temp->next;
    }
    currentlinenumber=maxlinenumber;
    temp->next=NULL;
    return head;
}
//create a text from a file
Text openFile(char* fn)
{
    head = (Text)malloc(sizeof(Line));
    Line *curr;
    Line *temp;
    curr=temp=head;
    FILE* fp = fopen(fn,"r");
    if(fp==NULL)
    {
        printf("file does not exist.\n");
        return NULL;
    }
    int lnumber=1;
    printf("Existing file \"%s\"\n",fn);
    char ch=fgetc(fp);
    if(ch==EOF)//if the file is empty
    {
        printf("Empty file\n");
    }
    else
    {
        rewind(fp);//set the file position to the start.
        while(!feof(fp))
        {
            char *current=(char*)malloc(sizeof(char)*LINELENGTH);
            memset(current,0,sizeof(char)*LINELENGTH);
            fgets(current,LINELENGTH,fp);
            if(strlen(current)>1)
                current[strlen(current)-1]='\0';//fgets() will receive the '\n' and this line will deal with the last '\n'
            curr = (Text)malloc(sizeof(Line));
            memset(curr,0,sizeof(Line));
            curr->data=current;
            curr->linenumber=lnumber++;
            maxlinenumber++;
            temp->next=curr;
            temp=temp->next;
        }
    }
    fflush(fp);
    fclose(fp);
    temp->next=NULL;
    return head;
}

void printLinkedList(Text t)
{
    if(t==NULL)
    {
        printf("Empty file\n");
        return;
    }
    Text tt=t;
    tt=tt->next;
    while(tt!=NULL)
    {
        if(tt->linenumber==currentlinenumber)
        {
            printf("--->\t%d:%s\n",tt->linenumber,tt->data);
        }
        else
            printf("Line\t%d:%s\n",tt->linenumber,tt->data);
        tt=tt->next;
    }
    return;
}
//when command is '+' or <Enter>,this function is called.
int increaseLineNumber(Text t)
{
    return currentlinenumber<maxlinenumber?++currentlinenumber:maxlinenumber;
}
//when command is '-' ,this function is called.
int decreaseLineNumber(Text t)
{
    return currentlinenumber>1?--currentlinenumber:1;
}

void insertLine(Text t)
{
    if(t==NULL)//if text does not exist,call createText()
    {
        t=createText();
        filechanged=1;
        filesaved=0;
        return;
    }
    Line *curr;
    Line *temp;
    Line *behind;
    int lnumber = currentlinenumber;
    if(currentlinenumber==1)//insert from first line.
    {
        temp=t;
        behind=t->next;
        while(1)
        {
            char *current=(char*)malloc(sizeof(char)*LINELENGTH);
            memset(current,0,sizeof(char)*LINELENGTH);
            gets(current);
            if(*current == '.')
                break;
            curr = (Text)malloc(sizeof(Line));
            memset(curr,0,sizeof(Line));
            curr->data=current;
            curr->linenumber=lnumber++;
            maxlinenumber++;
            temp->next=curr;
            temp=temp->next;
        }
        lnumber--;
        currentlinenumber=lnumber;
        temp->next=behind;
        // increase the line number behind current line.
        while(behind!=NULL)
        {
            behind->linenumber+=lnumber;
            behind=behind->next;
        }
    }else
    {
        temp=t->next;
        while(temp->linenumber<lnumber-1)
        {
            temp=temp->next;
        }
        behind=temp->next;
        while(1)
        {
            char *current=(char*)malloc(sizeof(char)*LINELENGTH);
            memset(current,0,sizeof(char)*LINELENGTH);
            gets(current);
            if(*current == '.')
                break;
            curr = (Text)malloc(sizeof(Line));
            memset(curr,0,sizeof(Line));
            curr->data=current;
            curr->linenumber=lnumber++;//insert,so the lnumber should less than current line number.
            maxlinenumber++;
            temp->next=curr;
            temp=temp->next;
        }
        currentlinenumber=lnumber-1;
        temp->next=behind;
        while(behind!=NULL)
        {
            behind->linenumber=lnumber++;
            behind=behind->next;
        }

    }
    //if you insert some line,the file was changed and file was not saved until you call saveFile() function.
    filechanged=1;
    filesaved=0;
}

//similar as the insertLine() function
void appendLine(Text t)
{
    if(t==NULL)
    {
        t=createText();
        filechanged=1;
        filesaved=0;
        return;
    }
    Line *curr;
    Line *temp;
    Line *behind;
    curr=temp=t->next;
    int lnumber=currentlinenumber;
    while(temp->linenumber<lnumber)
    {
        temp=temp->next;
    }
    behind=temp->next;
    while(1)
    {
        char *current=(char*)malloc(sizeof(char)*LINELENGTH);
        memset(current,0,sizeof(char)*LINELENGTH);
        gets(current);
        if(*current == '.')
            break;
        curr = (Text)malloc(sizeof(Line));
        memset(curr,0,sizeof(Line));
        curr->data=current;
        curr->linenumber=++lnumber;
        maxlinenumber++;
        temp->next=curr;
        temp=temp->next;
    }
    currentlinenumber=lnumber;
    lnumber=temp->linenumber;
    temp->next=behind;
    while(behind!=NULL)
    {
        behind->linenumber=++lnumber;
        behind=behind->next;
    }
    filechanged=1;
    filesaved=0;
}


void deleteLine(Text t)
{
    int lnumber=currentlinenumber;
    Line *temp;
    Line *behind;
    temp=t->next;
    //if linked list is null,you can not delete any line
    if(temp==NULL)
        return;
    if(lnumber==1)
    {
        t->next=temp->next;
        free(temp->data);
        free(temp);
        behind=t->next;
        while(behind!=NULL)
        {
            behind->linenumber--;
            behind=behind->next;
        }
        maxlinenumber--;
        if(currentlinenumber>maxlinenumber)
            currentlinenumber=maxlinenumber;
        return;
    }
    else
    {
        while(temp->linenumber<lnumber-1)
        {
            temp=temp->next;
        }
        behind=temp->next->next;
        free(temp->next->data);
        free(temp->next);
        temp->next=behind;
        while(behind!=NULL)
        {
            behind->linenumber--;
            behind=behind->next;
        }
        maxlinenumber--;
        if(currentlinenumber>maxlinenumber)
            currentlinenumber=maxlinenumber;
    }
    filechanged=1;
    filesaved=0;
}

void printTheLine(Text t,int number)
{
    if(t==NULL)
        return;
    Text tt=t;
    tt=tt->next;

    if(number<1 || number>maxlinenumber)
    {
        printf("Line number does not exits: command ignored\n");
        return;
    }
else
    {
        while(tt!=NULL && tt->linenumber<number)
        {
            tt=tt->next;
        }
        if(tt!=NULL)
        {
            printf("--->\t%d:%s\n",tt->linenumber,tt->data);
            currentlinenumber=number;
        }
        return;
    }
}
//after you call delete function,the printCurrentLine function is called.
void printCurrentLine(Text t)
{
    if(t==NULL)
        return;
    Text tt;
    tt=t->next;
    while(tt!=NULL && tt->linenumber<currentlinenumber)
    {
        tt=tt->next;
    }
    if(tt!=NULL)
        printf("--->\t%d:%s\n",tt->linenumber,tt->data);
    return;
}

void printHelp()
{
    printf("Commands are (in upper or lower case):\n");
    printf("\tq:\tquit\n");
    printf("\ts:\tsave\n");
    printf("\tx:\tforce exit\n");
    printf("\tf <filename>: the file is called <filename>\n");
    printf("\th:\tprint this help message\n");
    printf("\td:\tdelete current line\n");
    printf("\ta:\tappend after current line,terminated by '.'\n");
    printf("\ti:\tinsert before current line,terminated by '.'\n");
    printf("\tp:\tprint all lines\n");
    printf("\t.:\tprint current line\n");
    printf("\t+:\tincrement line and print\n");
    printf("\t<return>:same as '+'\n");
    printf("\t-:\tdecrement line and print\n");
    printf("\tnumber:\tmake 'number' the current line\n");
}

//free the data and then free text.
void quit(Text text)
{
        printf("bye\n");
        Text head=text;
        text=text->next;
        Text p=text;
        while(text!=NULL)
        {
            p=text->next;
            free(text->data);
            text->data=NULL;
            free(text);
            text=NULL;
            text=p;
        }
        free(head);
}
int isNumber(char *number)
{
    char *temp=number;
    while(*temp!='\0')
    {
        char i=*temp;
        if(i<'0' || i>'9')
        {
            return 0;
        }
        temp++;
    }
    return atoi(number);
}
char* setFilename(char* fn)
{
    hasfilename=1;
    return fn;
}

char* getFileNameFromCommand(char ch[],char* fn)
{
    if(!hasfilename)
    {
        strcpy(fn,ch+2);
        //fn=ch+2;
        FILE *fp=fopen(fn,"w");
        fclose(fp);
        printf("Creating file \"%s\"\n",fn);
        hasfilename=1;
    }
    else
    {
        printf("You have opened a file.\n");
    }
    return fn;
}

void saveFile(Text t,char* fn)
{
    if(!hasfilename)
    {
        printf("No filename. Use '-f<name>' command\n");
        return;
    }else
    {
        FILE *fp = fopen(fn,"w");
        Text tt=t;
        tt=tt->next;
        while(tt!=NULL)
        {
            if(tt->next==NULL)
            {
                fprintf(fp,"%s",tt->data);
                break;
            }
            else
            {
                fprintf(fp,"%s\n",tt->data);
            }
            tt=tt->next;
        }
        fflush(fp);
        fclose(fp);
        printf("Saving file \"%s\"\n",fn);
        filesaved = 1;
    }
}
//a loop waiting for command
void waitCommand(char *fn)
{
    char *fname=(char*)malloc(FNAMELENGTH*sizeof(char));
    memset(fname,0,sizeof(char)*FNAMELENGTH);
    if(hasfilename)
        strcpy(fname,fn);

    while(1)
    {
        char command[CMDLENGTH]="\0";
        printf("? ");

        //here the gets function will have some warning at some compilers because this function does not check the memory and the memory may leak.
        //but I cannot find another better function to receive a line from stdin,and using gcc it does works.
        gets(command);
        if(isNumber(command))
        {
            int n=isNumber(command);
            printTheLine(head,n);
        }else
        {
            if((command[0]=='h'||command[0]=='H') && command[1]=='\0')
            {
                printHelp();
            }
            else if((command[0]=='p'||command[0]=='P') && command[1]=='\0')
            {
                if(head!=NULL || hasfilename)
                {
                    printLinkedList(head);
                }
                else
                {
                    printf("Empty file\n");
                }
            }
            else if((command[0]=='q'||command[0]=='Q') && command[1]=='\0')
            {
                if(filechanged && !filesaved)
                {
                    printf("Cannot quit as file has changed. Use 'x' to force exit\n");
                }else
                {
                    quit(head);
                    free(fname);
                    fname=NULL;
                    break;
                }
            }
            else if((command[0]=='x'||command[0]=='X') && command[1]=='\0')
            {
                quit(head);
                free(fname);
                break;
            }
            else if((command[0]=='+'|| command[0]=='\0') && command[1]=='\0')
            {
                printTheLine(head,increaseLineNumber(head));
            }
            else if((command[0]=='-') && command[1]=='\0')
            {
                printTheLine(head,decreaseLineNumber(head));
            }
            else if((command[0]=='.') && command[1]=='\0')
            {
                printCurrentLine(head);
            }
            else if((command[0]=='i' || command[0]=='I') && command[1]=='\0')
            {
                insertLine(head);
            }
            else if((command[0]=='a' || command[0]=='A') && command[1]=='\0')
            {
                appendLine(head);
            }
            else if((command[0]=='d' || command[0]=='D') && command[1]=='\0')
            {
                deleteLine(head);
                printCurrentLine(head);
            }
            else if((command[0]=='s' || command[0]=='S') && command[1]=='\0')
            {

                saveFile(head,fname);
            }
            else if((command[0]=='f' || command[0]=='F') && command[1]==' ')
            {
                fname=getFileNameFromCommand(command,fname);
            }
            else
            {
                printf("Unknown command; ignoring\n");
            }
        }
    }
}


int main(int argc,char** argv)
{
    char *fname="\0";
    if(argc<=1)
    {
        hasfilename=0;
        filesaved=0;
    }
    else if(argc==2)
    {
        fname=argv[1];
        hasfilename=1;
        head=openFile(fname);
    }
    else
    {
        printf("too many arguments\n");
        return 0;
    }
    waitCommand(fname);
    return 0;
}
