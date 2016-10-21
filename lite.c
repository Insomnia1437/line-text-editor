#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define LINELENGTH 1000// max length of a line
#define FNAMELENGTH 100
#define CMDLENGTH 102
typedef struct _node
{
    int linenumber;
    char *data;
    struct _node *next;
}Line,*Text;

static int currentlinenumber=1;
char *fname="\0";
static int hasfilename=0;
static int filechanged=0;
static int filesaved=1;
static int maxlinenumber=0;
Text head;
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
        gets(current);
        if(*current == '.')
            break;
        curr = (Text)malloc(sizeof(Line));
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
Text openFile(char* fn)
{
    head = (Text)malloc(sizeof(Line));
    Line *curr;
    Line *temp;
    curr=temp=head;
    FILE* fp = fopen(fn,"r");
    if(fp==NULL)
    {
        printf("file can not open");
        return NULL;
    }
    int lnumber=1;
    printf("Existing file\"%s\"\n",fn);
    while(!feof(fp))
    {
        char *current=(char*)malloc(sizeof(char)*LINELENGTH);
        fgets(current,LINELENGTH,fp);
        current[strlen(current)-1]='\0';//fgets() will receive the '\n' and this line will deal with the last '\n'
        curr = (Text)malloc(sizeof(Line));
        curr->data=current;
        curr->linenumber=lnumber++;
        maxlinenumber++;
        temp->next=curr;
        temp=temp->next;
    }
    fclose(fp);
    temp->next=NULL;
    return head;
}

void printLinkedList(Text t)
{
    Text tt=t;
    tt=tt->next;
    if(tt==NULL)
    {
        printf("File is empty\n");
        return;
    }
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
}

int increaseLineNumber(Text t)
{
    return currentlinenumber<maxlinenumber?++currentlinenumber:maxlinenumber;
}

int decreaseLineNumber(Text t)
{
    return currentlinenumber>1?--currentlinenumber:1;
}

void insertLine(Text t)
{
    if(!hasfilename)
    {
        t=createText();
        return;
    }
    Line *curr;
    Line *temp;
    Line *behind;
    int lnumber = currentlinenumber;
    if(currentlinenumber==1)
    {
        temp=t;
        behind=t->next;
        while(1)
        {
            char *current=(char*)malloc(sizeof(char)*LINELENGTH);
            gets(current);
            if(*current == '.')
                break;
            curr = (Text)malloc(sizeof(Line));
            curr->data=current;
            curr->linenumber=lnumber++;
            maxlinenumber++;
            temp->next=curr;
            temp=temp->next;
        }
        lnumber--;
        currentlinenumber=lnumber;
        temp->next=behind;
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
            gets(current);
            if(*current == '.')
                break;
            curr = (Text)malloc(sizeof(Line));
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
    filesaved=0;
}

void appendLine(Text t)
{
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
        gets(current);
        if(*current == '.')
            break;
        curr = (Text)malloc(sizeof(Line));
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
    filesaved=0;
}


void deleteLine(Text t)
{
    int lnumber=currentlinenumber;
    Line *temp;
    Line *behind;
    temp=t->next;
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
    filesaved=0;
}

void printTheLine(Text t,int number)
{
    Text tt=t;
    tt=tt->next;
    Text p;
    if(number<=1)
    {
        printf("--->\t%d:%s\n",tt->linenumber,tt->data);
        currentlinenumber=1;
        return;
    }else if(number>=maxlinenumber)
    {
        while(tt!=NULL)
        {
            p=tt;
            tt=tt->next;
        }
        printf("--->\t%d:%s\n",p->linenumber,p->data);
        currentlinenumber=maxlinenumber;
        return;
    }else
    {
        while(tt->linenumber<number)
        {
            tt=tt->next;
        }
        printf("--->\t%d:%s\n",tt->linenumber,tt->data);
        currentlinenumber=number;
        return;
    }
}

void printCurrentLine(Text t)
{
    Text tt;
    tt=t->next;
    while(tt->linenumber<currentlinenumber)
    {
        tt=tt->next;
    }
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
void quit(Text text)
{
    if(filechanged && !filesaved)
    {
        printf("Cannot quit as file has changed.Use 'x' to force exit");
        return;
    }
    printf("bye\n");
    Text p=text;
    while(text!=NULL)
    {
        p=text->next;
        free(text->data);
        free(text);
        text=p;
    }
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

void getFileName(char* fn)
{
    if(!hasfilename)
    {
        //fn=ch+2;
        printf("Creating file \"%s\"\n",fn);
        FILE *fp=fopen(fn,"w");
        fclose(fp);
        hasfilename=1;
    }
    else
    {
        printf("You have opened a file.\n");
    }
}

void saveFile(Text t,char* fn)
{
    printf("fn:%s\n",fn);
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

void waitCommand()
{
    //char *fname=(char*)malloc(FNAMELENGTH*sizeof(char));
    while(1)
    {
        char command[CMDLENGTH]="\0";
        printf("? ");
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
                //printf("filename %s\n",anothername);
                if(head!=NULL||hasfilename)
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
                    break;
                }
            }
            else if((command[0]=='x'||command[0]=='X') && command[1]=='\0')
            {
                quit(head);
            }
            else if((command[0]=='+') && command[1]=='\0')
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
            }
            else if((command[0]=='s' || command[0]=='S') && command[1]=='\0')
            {
                saveFile(head,fname);
            }
            else if((command[0]=='f' || command[0]=='F') && command[1]==' ')
            {
                fname=command+2;
                getFileName(fname);
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
    if(argc<=1)
    {
        hasfilename=0;
        filesaved=0;
    }
    else
    {
        fname=argv[1];
        hasfilename=1;
        head=openFile(fname);
        //fname = "test.txt";
    }
    waitCommand();
    return 0;
}
