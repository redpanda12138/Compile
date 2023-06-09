#include <stdio.h>
#include <stdlib.h>
#include "string.h"

// ---------------- 特殊词表 ----------------------    
char *specialwords[] = {"main", "include", "define", "#"};

// ---------------- 关键词表 ----------------------
char *keywords[] = {"auto","double","int","struct","break","else","long","switch","case","enum","register",
        "typedef","char","extern","return","union","const","float","short","unsigned","continue","for",
         "signed","void","default","goto","sizeof","volatile","do","while","static","if"};

// ---------------- 运算符表 ----------------------
char *operators[] = {"+", "-", "*", "/", "%", ">", "<", ">=", "<=", "==","!=", "!", "&&", "||", "<<",
		 ">>", "~", "|", "^", "&", "=", "+=", "-=", "*=", "/=","%=", ",", "[]"};

// ---------------- 限界符表 ----------------------
char *boundwords[] = {";", "(", ")", "{", "}","\""};

// ---------------- 功能词表 ----------------------
char *functions[] = {"printf", "scanf", "memset"};

//---------------- 宏常量表 ----------------------
char title[50][20]={},content[50][60]={};
int type_list[50][20]={};

//---------------- 算符分析表 ----------------------
char *operator_list[] = {"#",",","=","+=", "-=", "*=", "/=","%=","||","&&",
"|", "^", "&", "==","!=",">",">=","<","<=",	"<<",">>","+", "-", "*", "/", "%","!","[]","(",")","i","N"
};
char operator_table[50][30]={};													//以字符为单位的算符列 
int  num_table[50][30]={};														//以算符标号为单位的算符列 

int  compare_table[31][31]=														//现在发现还是用hashmap储存优先级高效 
{
	0,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,100,-1,
	1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,1,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,100,-1,-1,1,-1,
	100,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,100,1,100,1,100,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,100,1,100,1,100
}; 

//---------------- 运算表 ----------------------
int start[50];					//记录长串中每个表达式的起始位置
int end[50]; 
char content_unit[][25]={} ;	//表达式分词后的长串 
int complete_num[50];
int analysis_stack[10];
char result[20][25];
char s1[20][30];
char s2[20][30];
char cal_stack[20][30];
int compare_tale_2[5][5]={
	0,0,-1,-1,-1,
	0,0,-1,-1,-1,
	1,1,0,0,0,
	1,1,0,0,0,
	1,1,0,0,0
};


//特殊词种别码
#define MAIN        1
#define INCLUDE     2
#define DEFINE      3
#define SIGN        4	//#
#define SPECIAL_DESC  "special"


//关键词种别码
#define AUTO       5
#define DOUBLE     6
#define INT        7
#define STRUCT     8
#define BREAK      9
#define ELSE       10
#define LONG       11
#define SWITCH     12
#define CASE       13
#define ENUM       14
#define REGISTER   15
#define TYPEDEF    16
#define CHAR       17
#define EXTERN     18
#define RETURN     19
#define UNION      20
#define CONST      21
#define FLOAT      22
#define SHORT      23
#define UNSIGNED   24
#define CONTINUE   25
#define FOR        26
#define SIGNED     27
#define VOID       28
#define DEFAULT    29
#define GOTO       30
#define SIZEOF     31
#define VOLATITLE  32
#define DO         33
#define WHILE      34
#define STATIC     35
#define IF         36
#define KEY_DESC  "keywords"


//标识符种别码
#define IDENTIFER 40
#define IDENTIFER_DESC "identifers"


//常量标种别码
#define INT_VAL     51   //整形常量
#define CHAR_VAL    52   //字符常量
#define FLOAT_VAL   53   //单精度浮点数常量
#define DOUBEL_VAL  54   //双精度浮点数常量
#define MACRO_VAL   55   //宏常量
#define CONSTANT_DESC "constant"


//运算符
#define ADD 80   			//+	
#define SUB 81   			//-
#define MUL 82   			//*
#define DIV 83   			///
#define MOD 84   			//%
#define GTEATER_THAN  74  	//>
#define GTEATER_EQUAL 75  	//>=
#define LESS_THAN     76  	//<
#define LESS_EQUAL    77  	//<=
#define EQUAL         72  	//==
#define NOT_EQUAL     73  	//!=
#define NOT           85  	//!
#define AND    		  68  	//&&
#define OR    		  67  	//||
#define LEFT_BIT      78  	//<<
#define RIGHT_BIT     79  	//>>
#define NEGATE        76  	//~ 按位取反 
#define OR_BIT        69  	//|
#define XOR_BIT       70  	//^
#define AND_BIT       71  	//&
#define EQU     61  		//=
#define ADD_EQU 62  		//+=
#define SUB_EQU 63  		//-=
#define MUL_EQU 64  		//*=
#define DIV_EQU 65  		///=
#define MOD_EQU 66  		//%=
#define COM     60  		//,
#define SQUARE  86  		//[]
#define OPERATOR_DESC "operators"


//限界符
#define SEM 		102 	 //;
#define DOUBLE_QUO  103  	 //"
#define COL 		104  	 //:
#define LEFT_PAR    87 	 //(
#define RIGHT_PAR   88 	 //)
#define LEFT_BRACE  107 	 //{ 
#define RIGHT_BRACE 108 	 //} 
#define QUO         109  	 //'
#define BOUNTWROD_DESC "boundwords"

//功能词
#define PRINT 140  //printf
#define SCAN 141   //scanf
#define MEM 142    //memset
#define FUNCTION_DESC   "functionwords"


#define NOTE1 150 // //注释//
#define NOTE2 151 // /*注释*/
#define NOTE_DESC "notes"


#define HEADER 160 // 头文件
#define HEADER_DESC "head"


#define MAX_CHAR_NUMBER   100000


char charSET[MAX_CHAR_NUMBER];

typedef struct NormalNode
{
    char content[50];
    char description[40];
    int  type;
    struct NormalNode *next;
}NormalNode;

typedef struct IdentiferNode
{
    char content[50];
    char description[40];
    int  type;
    struct IdentiferNode *next;
}IdentiferNode;


NormalNode *normalHead;
IdentiferNode *identiferHead;


void initNode()
{
    normalHead = (NormalNode *)malloc(sizeof(NormalNode));
    strcpy(normalHead->content,  "");
    strcpy(normalHead->description, "");
    normalHead->type = -1;
    normalHead->next = NULL;
    
    identiferHead = (IdentiferNode *)malloc(sizeof(IdentiferNode));
    strcpy(identiferHead->content, "");
    strcpy(identiferHead->description, "");
    identiferHead->type = -1;
    identiferHead->next = NULL;
}


void createNewNode(char *content,char *describe,int type)
{
    NormalNode *p = normalHead;
    NormalNode *tmp = (NormalNode *)malloc(sizeof(NormalNode));
    
    while ( p->next != NULL) {
        p = p->next;
    }
    
    strcpy(tmp->content,content);
    strcpy(tmp->description,describe);
    tmp->type = type;
    tmp->next = NULL;   
    
	p->next = tmp;
}


int createIdeNode(char *content,char *describe,int type)
{
	
    IdentiferNode *p = identiferHead;
    IdentiferNode *tmp = (IdentiferNode *)malloc(sizeof(IdentiferNode));
   

    while (p->next != NULL){
        p = p -> next;
        if(!strcmp(p->content, content))          //标识符已经存在
            return 0;
    }
    strcpy(tmp->content,content);
    strcpy(tmp->description,describe);
    tmp->type = type;
    tmp->next = NULL;
    
    p->next= tmp;  
    return 0;
}


void displaynormalNode(int n)
{
    NormalNode *p = normalHead;
    int p_flag = 0,m_flag = 0,k=0,i=0;	
    if(n==1)
    {
		while ( (p = p -> next)) {
        printf("(");		
		printf("%s",p->content);
		printf(",");
        printf("%s",p->description);
        printf(",");
        printf("%d",p->type);
		printf(")");
		printf("\n");
    	}
    }
    else
	{
		FILE *fp;
    	fp = fopen("./result.txt", "a");
    	while(p = p->next)
 		{
	 		if(p->type == SIGN){													//遇到#号 
	 		printf("\n");
	 		printf("%s ",p->content); 
	 		p_flag = 0;
	 		fprintf(fp,"\n");
 			fprintf(fp,"%s ",p->content);
	 		}
	 	else if(p->type == MAIN) 
		 {
		 	m_flag = 1;
		 	p_flag = 0;
		 	printf("%s ",p->content);
		 	fprintf(fp,"%s ",p->content);
		 }
		 else if(p->type == DEFINE) 											//遇到define 
		 {
		 	printf("%s ",p->content);
		 	fprintf(fp,"%s ",p->content);
		 }		
		else if(p->type == MACRO_VAL && p_flag == 0 && m_flag == 0)
 		{
		 	p_flag = 1;
		 	printf("%s ",title[k]);
			printf("%s ",result[k]);
			fprintf(fp,"%s ",title[k]);
			fprintf(fp,"%s ",result[k]);		
			k++;
	 	}	 
		 else if(p->type == MACRO_VAL && m_flag == 1 )
 		{		 	
			for(i=0;;i++)
			{
				if(strcmp(title[i],p->content)==0) break;
			}
			printf("%s ",result[i]);
			fprintf(fp,"%s ",result[i]);		
	 	}	 											
	 	else if(p_flag == 1)
	 	{
	 		continue;
		}
		else if(p->type==SEM && p_flag == 0)
	 	{
	 		printf("%s ",p->content);
	 		printf("\n");
	 		fprintf(fp,"%s ",p->content);
	 		fprintf(fp,"\n");
		}
		else
		{
			printf("%s ",p->content);
			fprintf(fp,"%s ",p->content);
		}
	 	
 		}
 		fclose(fp); 
    }
}


//返回关键词或标识符 
int matchID(char *word)
{
	int i; 
    for (i = 0; i < 32; i++) {
        if(strcmp(word, keywords[i]) == 0){	
			return i+1;										//返回关键词 
        }
    }
    for (i = 0; i < 50; i++) {
        if(strcmp(word, title[i]) == 0){		
            return MACRO_VAL;
        }
    }
    if(strcmp(word, "printf") == 0) return PRINT;
    else if (strcmp(word, "scanf") == 0) return SCAN;
    else if (strcmp(word, "memset") == 0) return MEM;
    return IDENTIFER;
}

//预处理 
int preProcess(char *word,char title[][20],char content[][60],int *define_n)
{
    const char *head_define = "define";
    const char *head_include = "include";
    char * p_include,*p_define;
    int flag = 0, list = 0;
    p_include = strstr(word,head_include);										//判断include是否是word的子串 
    if(p_include!=NULL){
    	createNewNode((char*)head_include,SPECIAL_DESC,INCLUDE);
        flag = 1;
        int i;
        for(i=7;;)
        {
            if(*(p_include+i) == ' ' || *(p_include+i) == '\t')					//判断以排除空格和tab 
            {
                i++;
            }
            else
            {
                break;
            }
        }
        createNewNode(p_include+i,HEADER_DESC,HEADER);
        return -1;
    }
    else
	{
        p_define = strstr(word,head_define);									//判断define是否是word的子串 	
        if(p_define!=NULL)
        {
        	
        	//printf("%d",*define_n);
        	createNewNode((char*)head_define,SPECIAL_DESC,DEFINE);
            flag = 1;
            int i,j,k,count_ch;
            for(i=7;;)
            {
                if(*(p_define+i) == ' ' || *(p_define+i) == '\t')				//判断以跳过空格和tab 
                {
                    i++;
                }
                else
                {
                    break;
                }
            }
            
            for(j=i;;)
            {		
                if(*(p_define+j) != ' ' && *(p_define+j) != '\t')				//判断以跳过宏变量名 
                {
                    j++;					
                }
                else				
                {
                    break;
                }
            }
           
		    for(k=j;;)
            {
                if(*(p_define+k) == ' ' || *(p_define+k) == '\t')				//判断以再次跳过空格和tab 
                {
                    k++;
                }
                else
                {
                    break;
                }
            }
            
            
            createNewNode(p_define+i,p_define+k,MACRO_VAL);
            list = 0;
            for(count_ch=i;count_ch<j;count_ch++)
			{	
				//putchar(**title);
				title[*define_n][list] = *(p_define+count_ch);
				title[*define_n][list+1] = '\0';
				list++;
            	//putchar(*(p_define+count_ch));
            }
            
            
			list = 0;
			for(count_ch=k;word[count_ch];count_ch++)
			{
				content[*define_n][list] = *(p_define+count_ch);
				content[*define_n][list+1] = '\0';
				list++;
            	//putchar(*(p_define+count_ch));
            }
            *define_n=*define_n+1;
            //printf("\n");
            
        	return (count_ch-k)*(-1);
        }
        
    }
}


//获取算符
int getOperator(){
	 NormalNode *p = normalHead;
	 int i = -1,j = 0, k = 0, n = 0,p_flag = 0,s_count = 0, first_flag = 0;
	 char str[30];
	 while(p = p->next)
 	{
	 	if(p->type == SIGN){													//遇到#号 
	 		operator_table[i][j]='\0';
		 	p_flag = 0;
		 	j = 0;
		 	k = 0;
	 	}
	 	else if(p->type == MAIN) 
		 {
		 	if(s_count!=0)
		 	{ 
		 		strcpy(content_unit[s_count],"end");
		 		end[n] = s_count-1;
		 		s_count++;
		 		n++;
		 	} 
			 break;
		 }
		else if(p->type == MACRO_VAL && p_flag == 0 )
 		{
		 	p_flag = 1;
		 	first_flag = 1;
		 	continue;		
	 	}
	 	else if(p->type == DEFINE) 												//遇到define 
		 {
		 	i++;
		 	if(s_count!=0)
		 	{ 
		 		strcpy(content_unit[s_count],"end");
		 		end[n] = s_count-1;
		 		s_count++;
		 		n++;
		 	} 
		 }									
	 	else if(p_flag == 1)
	 	{
	 		if(p->type == MACRO_VAL || strcmp(p->description,CONSTANT_DESC)==0)		//内容为宏或者常数 
	 		{
		 		operator_table[i][j] = 'i';
		 		num_table[i][k]=30;
		 		type_list[i][k]=p->type;
				
				if(first_flag==1)												//记录每个首元素的序号 
				{
					start[n] = s_count;
					first_flag = 0;	
				}
				 strcpy(str,p->content);
				 //puts(str);
				 strcpy(content_unit[s_count],str);
				 //printf("%d ",s_count );
				 //puts(content_unit[s_count]);
		 		 s_count++;
		 						 
			 	k++;
			 	j++; 
		 	}
		 	else if(strcmp(p->description,OPERATOR_DESC)==0 || strcmp(p->description,BOUNTWROD_DESC)==0) //操作符或边界符号 
		 	{
			 	if(strlen(p->content)==1)	operator_table[i][j] = *p->content;
			 	else 
			 	{
	 				operator_table[i][j] = p->content[0];
	 				j++;
	 				operator_table[i][j] = p->content[1];
	 			}
	 			num_table[i][k] = (p->type)-59;
	 			type_list[i][k]=p->type;		
				
				strcpy(str,p->content);
			    //puts(str);
				 strcpy(content_unit[s_count],str);
				 
				 //printf("%d ",s_count );
				 //puts(content_unit[s_count]);
		 		 s_count++;
		 		
			 	j++;
			 	k++;
		 	} 
	 	}
	 	
 	}
 	//puts(operator_table[1]);
 	for(i=0;num_table[i][0];i++)
 	{
	 /*for(j=0;num_table[i][j];j++)
	 	{
	 		//num_table[i][j] -= 60;
		 	printf("%d ",num_table[i][j]);	
	 	}*/
	 	printf("\n");
	 }
 	return n; 
} 


//算符优先分析
int operatorPrecedence()
{
	int step = 0,symbolStack[30]={-1},wait = -1,flag_count = 0,flag_key = 0 ,flag_2 = 0, i = 0, j = 0, k = 0;
	char prior;
	char action[10]={};
	printf("语法分析结果为: \n\n");	
	for(i=0;num_table[i][0];i++)
	{
		printf("待分析的宏常量:");	puts(title[i]);    
		printf("待分析的表达式:");  puts(content[i]);  
		printf("转换后的表达式:");  puts(operator_table[i]);  printf("\n");
		printf("step	symbolStack 		prior	  waitInput  		restString		action");
		printf("\n\n");
		for(j=0;j<30;j++) symbolStack[j]=-1;
		for(j=0;j<10;j++) action[j]='\0';
		symbolStack[0]=0;
		step = 0;wait = -1;flag_count = 0;flag_key = 0;flag_2 = 0; j = 0; k = 0; prior = '\0';
		for(flag_2 = 0;;)
	 	{
	 		if(wait == -1) 
		 	{
		 		wait = num_table[i][flag_2];
			 	flag_2++;
		 	}
			 
	 		if(compare_table[symbolStack[flag_key]][wait] < 0)			//出现'<'  
 			{
				prior = '<';
				strcpy(action,"移进"); 
		 	}
		 	//右括号在wait时'='
		 	else if(compare_table[symbolStack[flag_key]][wait] == 0 && wait == 29)			 
 			{
				prior = '=';
				strcpy(action,"移进"); 
		 	}
		 	//出现'>' 且flag不指向右括号 
	 		else if(compare_table[symbolStack[flag_key]][wait] > 0 && symbolStack[flag_key] != 29)	
	 		{
		 		prior = '>';
		 		strcpy(action,"规约");
		 	}
		 	//出现'>' 且flag指向右括号 
	 		else if(compare_table[symbolStack[flag_key]][wait] > 0 && symbolStack[flag_key] == 29)
	 		{
		 		prior = '>';
		 		strcpy(action,"规约");
		 	}
		 	//#号在wait时出现'=' 
		 	else if(compare_table[symbolStack[flag_key]][wait] == 0 && wait == 0)
	 		{
		 		prior = '=';
		 		strcpy(action,"接受");
		 	}
	 		else
	 		{
		 		prior = ' ';
		 		strcpy(action,"错误");
		 	}

		 	//输出这一行的分析栈 
	 		printf("%d	",step++);
	 		for(k=0;symbolStack[k]>=0;k++)	printf("%s",operator_list[symbolStack[k]]);
	 		printf("	 		");
	 		printf("%c		",prior);
	 		printf("%s	 	",operator_list[wait]);
	 		for(k=flag_2;num_table[i][k]>0;k++)	printf("%s",operator_list[num_table[i][k]]);
	 		if(wait != 0)printf("%s",operator_list[num_table[i][k+1]]);
	 		printf("			");
	 		puts(action);
	 		

			//出现'<'  
	 		if(compare_table[symbolStack[flag_key]][wait] < 0)								
 			{
			 	flag_count++;
				symbolStack[flag_count] = wait;
				wait =  num_table[i][flag_2];
				flag_2++;	
				flag_key = flag_count;
			
		 	}
		 	//右括号在wait时'='
		 	else if(compare_table[symbolStack[flag_key]][wait] == 0 && wait == 29)		
 			{
			 	flag_count++;
				symbolStack[flag_count] = wait;
				wait =  num_table[i][flag_2];
				flag_2++;	
				flag_key = flag_count;
				
		 	}
		 	//出现'>' 且flag不指向右括号 
	 		else if(compare_table[symbolStack[flag_key]][wait] > 0 && symbolStack[flag_key] != 29)	
	 		{
		 		for(flag_key=flag_key-1;;)
		 		{
		 			if(symbolStack[flag_key]==31) flag_key--;
		 			else break;
		 		}
		 		symbolStack[flag_key+1] = 31;
		 		for(k=flag_key+2;symbolStack[k]>=0;k++)  symbolStack[k] = -1;	
		 		flag_count = flag_key + 1;
		 	
		 	}
		 	//出现'>' 且flag指向右括号 
	 		else if(compare_table[symbolStack[flag_key]][wait] > 0 && symbolStack[flag_key] == 29)
	 		{
		 		for(flag_key=flag_key-1;;)
		 		{
		 			if(symbolStack[flag_key]==31 ) flag_key--;
		 			else if( symbolStack[flag_key]==28) flag_key--;
		 			else break;
		 		}
		 		symbolStack[flag_key+1] = 31;
		 		for(k=flag_key+2;symbolStack[k]>=0;k++)  symbolStack[k] = -1;
		 		flag_count = flag_key + 1;
		 		
		 	}
	
	
		 	if(strcmp(action,"接受")==0 || strcmp(action,"错误")==0) break;
	 	}
	 	printf("\n\n");
	}
	
}


int push(n)
{
	int i;
	for(i=0;analysis_stack[i];i++);
	analysis_stack[i]=n;
	return i;
}

int pop()
{
	int i,tem;
	for(i=0;analysis_stack[i];i++);
	i--;
	tem = analysis_stack[i];
	analysis_stack[i]='\0';
	i--;
	return tem;	
}

int push_s(char s[],int n)
{
	int i=0;
	if(n==1)
	{
		for(i=0;strcmp(s1[i],"")!=0;i++);		
		strcpy(s1[i],s);
		strcpy(s1[i+1],"");
	}
	else if(n==2)
	{
		for(i=0;strcmp(s2[i],"")!=0;i++);		
		strcpy(s2[i],s);
		strcpy(s2[i+1],"");
	}	
	else if(n==3)
	{
		for(i=0;strcmp(cal_stack[i],"")!=0;i++);		
		strcpy(cal_stack[i],s);
		strcpy(cal_stack[i+1],"");
	}	
	return i;
} 

char* pop_s(int n)
{
	int i=0;
 	static char s[25];
	if(n==1)
	{
		for(i=0;strcmp(s1[i],"")!=0;i++);
		i--;
		strcpy(s,s1[i]);
		strcpy(s1[i],"");
		//puts(s);
	}
	else if(n==2)
	{
		for(i=0;strcmp(s2[i],"")!=0;i++);
		i--;
		strcpy(s,s2[i]);
		strcpy(s2[i],"");
	}	
	else if(n==3)
	{
		for(i=0;strcmp(cal_stack[i],"")!=0;i++);
		i--;
		strcpy(s,cal_stack[i]);
		strcpy(cal_stack[i],"");
		
	}	
	return s;
}


int substitute(int n){
	
	int temp, i=0, m=0, j=0, k=0, complete_state = 1, ana = 0;
	char str[100]="";
	char string[15] = "";
	for(i=0;i<n;i++) 
		{
			strcat(str,title[i]);
			strcat(str," ");
		}
	
	
	for(m=0;m<n;m++)
	{
		if(start[m]!=end[m]) continue;
		else 
		{
			complete_num[m]=1;													//存入已是数值的宏 
			strcpy(result[m],content_unit[start[m]]); 
		}
		for(i=0;i<end[n-1];i++)													//找出已定义数值宏 
		{
			if(strcmp(content_unit[i],title[m])==0) 
				strcpy(content_unit[i],content_unit[start[m]]);  				//用已知数值宏替换 
		}
	}
	
		while(1)
		{
			for(i=0;i<n;i++)
			{
				if(complete_num[i]==0)
				{
					for(j=start[i];j<end[i];j++)
					{
						if(strstr(str,content_unit[j])) complete_state=0;		//包含宏名 
						else continue;
					}
				}
				else continue;
				
				if(complete_state == 1) push(i);								//将可计算的宏入栈 
				else complete_state =1;
			} 
			
			//for(i=0;analysis_stack[i];i++) printf("%d ",analysis_stack[i]);
			printf("\n");
			while(analysis_stack[0])
			{
				ana = pop();
				compute(ana);
			}
			
			
			complete_state = 1;												//全部完成就退出 
			for(i=0;i<n;i++)
			{
				if(complete_num[i]==0) complete_state = 0;	
			} 
			if(complete_state == 1) break;										
		}
	
	return 0;
}

int compute(ana)
{
	int i=0, j=0, k=0 ,t=0, m=0, n=0;
	char* str,* num_1,* num_2,* nn;
	
	
	for(i=start[ana];i<=end[ana];)
	{
		if(num_table[ana][i-start[ana]] == 30) 									//遇到数值 
		{
			push_s(content_unit[i],2);
			i++;
		}			
		else if(num_table[ana][i-start[ana]] == 28) 
		{
			push_s(content_unit[i],1);
			i++;
		}
		else if(num_table[ana][i-start[ana]] == 29)
		{
			for(j=0;strcmp(s1[j],"")!=0;j++);	
			j--;
			while(strcmp(s1[j],"(")!=0)
			{
				str=pop_s(1);
				push_s(str,2);
				j--;
			}
			pop_s(1);
			i++;
		}
		else 
		{
			for(j=0;strcmp(s1[j],"")!=0;j++);	
			j--;		

			m = num_table[ana][i-start[ana]]-21;
			switch(s1[j][0])
			{
				case '+': n=0;break;
				case '-': n=1;break;
				case '*': n=2;break;
				case '/': n=3;break;
				case '%': n=4;break;
			}
			//printf("%d ",n);
			
			if (strcmp(s1[0],"")==0 || strcmp(s1[j],"(")==0 || compare_table_2[m][n]>0)
			{
				push_s(content_unit[i],1);				
				i++;
			}
			else 
			{
				str=pop_s(1);
			 	push_s(str,2);
			}
		}		
	}
    
	while(strcmp(s1[0],"")!=0) 
	{
 		str=pop_s(1);
		push_s(str,2);
 	}


	
	//for(i=0;strcmp(s2[i],"")!=0;i++) puts(s2[i]);
	
	int r = 0;
	for(i=0;strcmp(s2[i],"")!=0;i++) 
	{
		//puts(s2[i]);
		if(strstr(s2[i],"0")||strstr(s2[i],"1")||strstr(s2[i],"2")||strstr(s2[i],"3")||strstr(s2[i],"4")||strstr(s2[i],"5")||strstr(s2[i],"6")||strstr(s2[i],"7")||strstr(s2[i],"8")||strstr(s2[i],"9"))
		{
			push_s(s2[i],3);
			//puts(s2[i]);
		}
		else if(strstr(s2[i],"+")||strstr(s2[i],"-")||strstr(s2[i],"*")||strstr(s2[i],"/")||strstr(s2[i],"%"))
		{
			
			num_1 = pop_s(3);
			strcpy(num_2,num_1);
			num_1 = pop_s(3);			
			
			switch(s2[i][0])
			{
				case '+': r = transfer_i(num_1)+ transfer_i(num_2);break;
				case '-': r = transfer_i(num_1)- transfer_i(num_2);break;
				case '*': r = transfer_i(num_1)* transfer_i(num_2);break;
				case '/': r = transfer_i(num_1)/ transfer_i(num_2);break;
				case '%': r = transfer_i(num_1)% transfer_i(num_2);break;
				default: printf("含有非法字符");
			}
			sprintf(str,"%#x",r);
			//puts(str);
			push_s(str,3);
		}
	}
	//printf("结果是:");
	//puts(cal_stack[0]);
	//printf("\n");
	strcpy(result[ana],cal_stack[0]);
	complete_num[ana]=1;
	
	while(strcmp(s1[0],"")!=0) 													//清空 s1 
	{
 		pop_s(1);
 	}
 	while(strcmp(s2[0],"")!=0) 													//清空 s2
	{
 		pop_s(2);
 	}
 	while(strcmp(cal_stack[0],"")!=0) 											//清空 cal_stack
	{
 		pop_s(3);
 	}
}


int transfer_i(char*s)
{
	int temp;	
	if(strstr(s,"0x")) sscanf(s,"%x",&temp);
	else sscanf(s,"%d",&temp);
	//printf("%#x",temp);
	return temp;
}

float transfer_f(int n)
{
	float temp;	
    sscanf(content_unit[n],"%f",&temp);
	//printf("%#x",temp);
	return temp;
}





int main(){
    
    initNode();
    FILE *fp;
    char ch,ch_t;
    int count = 0, define_n = 0,i = 0, j = 0, len=0, total=0;
    char tmp[100];
    char *word ;   											//  保存description
    
    if((fp = fopen("./options.txt", "r")) == NULL){
        printf("文件不存在");
    }
	
	printf("词法分析结果为:\n") ; 
    while( ch!= EOF){
        ch = fgetc(fp);
        //putchar(ch);
        
        
        //处理注释
        if (ch == '/') {
            ch = fgetc(fp);
            
            //处理    //类型
            if(ch == '/') {
                while (ch != ' ' && ch != '\n') {
                    ch = fgetc(fp);
                }
                createNewNode("空", NOTE_DESC, NOTE1);
            }
            
            //处理    /*类型注释*/
            else if(ch == '*'){
                ch = fgetc(fp);
                while (ch != '*'){
                    ch = fgetc(fp);
                    if(ch == '\n'){
                    }
                }while (ch != '/'){
                    ch = fgetc(fp);
                }
                createNewNode("空", NOTE_DESC, NOTE2);
            }
            
            //处理 运算符
            else {
                createNewNode("/", OPERATOR_DESC, DIV);
            }
        }
        
        //处理关键字和标识符
        if((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_'){
            count = 0;
            while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_' || (ch >= '0' && ch<= '9')) {
                tmp[count++] = ch;
                ch = fgetc(fp);
            }
            word = (char *)malloc(sizeof(char)*count);
            memcpy(word, tmp, count);											//(源,目标,字符数)
            word[count] = '\0';
            int id = matchID(word);
            if (strcmp(word,"main") == 0) 										//判断main 
            	createNewNode(word, SPECIAL_DESC, MAIN);
			else if(id >= 140 && id <= 143){
				createNewNode(word,FUNCTION_DESC, id);							//判断功能词 
			}	 
			else if(id == MACRO_VAL){	
		 		createNewNode(word,"MACRO", MACRO_VAL);							//判断宏定义 
			}	           	
            else if (id == IDENTIFER ) {
                	int address_id = createIdeNode(word, IDENTIFER_DESC, IDENTIFER);
                	createNewNode(word, IDENTIFER_DESC, IDENTIFER);
            		}
				else{
  					createNewNode(word, KEY_DESC, id);
           			}
            fseek(fp,-1L,SEEK_CUR);//向后回退一位
        }
        
        
        //处理数字常量
        else if(ch >= '0' && ch <= '9' ){
            count = 0;
            int isFloat = 0;
            int isDouble = 0;
            while (ch >= '0' && ch <= '9'|| ch=='x') {							//处理16进制数 
                tmp[count++] = ch;
                ch = fgetc(fp);
            }
            
            //处理float
            if (ch == '.') {
                isFloat = 1;
                tmp[count++] = ch;
                ch = fgetc(fp);
                if (ch >= '0' && ch <= '9') {
                    while (ch >= '0' && ch <= '9' ){
                        tmp[count++] = ch;
                        ch = fgetc(fp);
                    }
                }else{
                    isFloat = -1;      											//出错，-1表示 小数点后没有数字.
                }
            }
            
            //处理double
            if(ch == 'E' || ch == 'e'){
                isDouble = 1;
                tmp[count++] = ch;
                ch = fgetc(fp);
                if (ch == '+' || ch == '-') {
                    tmp[count++] = ch;
                    ch = fgetc(fp);
                }
                if (ch >= '0' && ch <= '9') {
                    tmp[count++] = ch;
                    ch = fgetc(fp);
                }else{
                    isDouble = -1;    //出错，-1表示 E后面的格式不对.
                }
            }
            
            word = (char *)malloc(sizeof(char)*(count+1));
            memcpy(word, tmp, count);
            word[count] = '\0';
            if (isFloat==1) {
                createNewNode(word, CONSTANT_DESC, FLOAT_VAL);
            }else if(isDouble==1){
                createNewNode(word, CONSTANT_DESC, DOUBEL_VAL);
            }else if(isFloat!=-1 && isDouble!=-1){
                createNewNode(word, CONSTANT_DESC, INT_VAL);
            }
            fseek(fp,-1L,SEEK_CUR);//向后回退一位
        }
        
        //处理换行
        else if(ch == ' ' || ch == '\t' || ch == '\r' || ch =='\n' ){
            if(ch == '\n'){
                
            }
        }
        
        //处理头文件和宏常量(预处理)
        else if(ch == '#'){
            createNewNode("#", SPECIAL_DESC, SIGN);
			count = 0;
            while(ch!='\n' && ch!=EOF)
            {
                tmp[count++] = ch;
                ch = fgetc(fp);
            }
            word = (char *)malloc(sizeof(char)*(count+1));
            memcpy(word,tmp,count);
            word[count] = '\0';
            len = preProcess(word,title,content,&define_n);
            
            fseek(fp,len-1,SEEK_CUR);
           
        }
    
        else if(ch == '*'){
        	ch_t = fgetc(fp);
        	if(ch_t == '=') {createNewNode("*=", OPERATOR_DESC, MUL_EQU);}
        	else {createNewNode("*", OPERATOR_DESC, MUL);fseek(fp,-1L,SEEK_CUR);}
  	
        }
        
        else if(ch == '+'){
        	ch_t = fgetc(fp);
        	if(ch_t == '=') {createNewNode("+=", OPERATOR_DESC, ADD_EQU);}
        	else {createNewNode("+", OPERATOR_DESC, ADD);fseek(fp,-1L,SEEK_CUR);}
  	
        }
        
         else if(ch == '-'){
        	ch_t = fgetc(fp);
        	if(ch_t == '=') {createNewNode("-=", OPERATOR_DESC, SUB_EQU);}
        	else {createNewNode("-", OPERATOR_DESC, SUB);fseek(fp,-1L,SEEK_CUR);}
  	
        }
        
        else if(ch == '%'){
        	ch_t = fgetc(fp);
        	if(ch_t == '=') {createNewNode("%=", OPERATOR_DESC,MOD_EQU);}
        	else {createNewNode("%", OPERATOR_DESC, MOD);fseek(fp,-1L,SEEK_CUR);}
  	
        }
        
        else if(ch == '='){            
           	ch_t = fgetc(fp);
        	if(ch_t == '=') {createNewNode("==", OPERATOR_DESC, EQUAL);}
        	else {createNewNode("=", OPERATOR_DESC, EQU);fseek(fp,-1L,SEEK_CUR);}
        }
        
        else if(ch == '&'){            
           	ch_t = fgetc(fp);
        	if(ch_t == '&') {createNewNode("&&", OPERATOR_DESC, AND);}
        	else {createNewNode("&", OPERATOR_DESC, AND_BIT);fseek(fp,-1L,SEEK_CUR);}
        }
        
        else if(ch == '|'){            
           	ch_t = fgetc(fp);
        	if(ch_t == '|') {createNewNode("||", OPERATOR_DESC, OR);}
        	else {createNewNode("|", OPERATOR_DESC, OR_BIT);fseek(fp,-1L,SEEK_CUR);}
        }
        
        else if(ch == '!'){            
           	ch_t = fgetc(fp);
        	if(ch_t == '=') {createNewNode("!=", OPERATOR_DESC, NOT_EQUAL);}
        	else {createNewNode("!", OPERATOR_DESC, NOT);fseek(fp,-1L,SEEK_CUR);}
        }
        
        else if(ch == '>'){           
            ch_t = fgetc(fp);
        	if(ch_t == '=') {createNewNode(">=", OPERATOR_DESC, GTEATER_EQUAL);}
        	else if(ch_t == '>') {createNewNode(">>", OPERATOR_DESC, RIGHT_BIT);}
        	else {createNewNode(">", OPERATOR_DESC, GTEATER_THAN);fseek(fp,-1L,SEEK_CUR);}
        }
        
        else if(ch == '<'){          
            ch_t = fgetc(fp);
        	if(ch_t == '=') {createNewNode("<=", OPERATOR_DESC, LESS_EQUAL);}
        	else if(ch_t == '<') {createNewNode("<<", OPERATOR_DESC, LEFT_BIT);}
			else {createNewNode("<", OPERATOR_DESC, LESS_THAN);fseek(fp,-1L,SEEK_CUR);}
        }
        
        else if(ch == '^'){
            createNewNode("^", OPERATOR_DESC, XOR_BIT);
        }
        
        else if(ch == ','){
            createNewNode(",", OPERATOR_DESC, COM);
        }
        
        else if(ch == ';'){
            createNewNode(";", BOUNTWROD_DESC, SEM);
        }
        
        
        else if(ch == '"'){
            createNewNode("\"", BOUNTWROD_DESC, DOUBLE_QUO);
        }
        
		else if(ch == '\''){
            createNewNode("'", BOUNTWROD_DESC, QUO);
        }
        
        else if(ch == ':'){
            createNewNode(":", BOUNTWROD_DESC, COL);
        }
        
        else if(ch == '('){
            createNewNode("(", BOUNTWROD_DESC, LEFT_PAR);
        }
        
        else if(ch == ')'){
            createNewNode(")", BOUNTWROD_DESC, RIGHT_PAR); 
        }
        
		else if(ch == '{'){
            createNewNode("{", BOUNTWROD_DESC, LEFT_BRACE);
        }
        
        else if(ch == '}'){
            createNewNode("}", BOUNTWROD_DESC, RIGHT_BRACE);
        }
        
        
        
        if (ch != ' '&& ch != '\n') {   //去掉空格和换行符
            charSET[count] = ch;
            count++;
        }
        count = 0;
    }
    fclose(fp);    
    displaynormalNode(1);
    //puts(title[2]);
    //puts(content[2]);
    
    total = getOperator();
	operatorPrecedence();
	//for(i=start[5];i<=end[5];i++) puts(content_unit[i]);
//	printf("%d ",total);
	
	substitute(total);
	

	printf("计算结果为:\n\n");
	for(i=0;i<=total;i++)
		{
			puts(title[i]);
			puts(result[i]);  
			printf("\n");
		}
		
	printf("宏替换后程序如下:");
	displaynormalNode(2);
	printf("\n\n");
	printf("程序已输出至result.txt\n\n");
}
