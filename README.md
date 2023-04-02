# Compile
一个C语言宏替换程序

###软件架构
软件架构说明

C源程序和翻译后的程序如下：

源程序 
#include <stdio.h> 
#define ADDR_START 0x20480000 
#define ADDR_A ADDR_START + 0x04 
#define ADDR_B ADDR_START + 15
#define BUFFER_SIZE 1024 
#define BUFFER_END ADDR_START + BUFFER_SIZE – 1 
#define PI 3.14 
void main() 
{
 float r, l; 
 scanf(“%f”, &r);
 l = 2 * PI *r;
 memset(ADDR_START, BUFFER_SIZE, 0x00); 
}

运行后生成文件 
#include <stdio.h> 
#define ADDR_START 0x20480000 
#define ADDR_A 0x20480004 
#define ADDR_B 0x2048000F 
#define BUFFER_SIZE 1024 
#define BUFFER_END 0x204803FF 
#define PI 3.14 
void main()
{ 
 float r, l;
 scanf(“%f”, &r);
 l = 2 * 3.14 *r;
 memset(0x20480000, 1024, 0x00); 
}

###概要设计
首先程序进行词法分析，词法分析程序的主要流程是对源程序语句从左到右扫描，逐个读入源程序字符并按照构词规则划分成多个独立表意的单词，再确定该单词的属性分类
（如关键字、宏常量、标识符等）。

词法分析中将单词分为以下几类：
1.关键字keywords：由程序语言定义的保留字。如auto、short、typedef等。 
2.宏常量MACRO：本次实验的主要目的就是分析宏常量，所以单独定义，但在总分类 时还是作为constant。(但定义时并不直接标出，因为在描述中呈现的是该宏常量的内容) 
3.一般变量identifers：用来表示程序中各种名字的字符串。
4.常数constant：常数的类型一般有整型、实型、布尔型、文字型。
5.运算符operators：如+、- 、*、/等。
6.界限符boundwords：如逗号、分号、括号等。
7.特殊字符specialwords：C语言在语法上并未将main、include以及define等符号定义为 关键字，所以单独列出。

  词法分析得到的结果是一个初始符号表，每一个表项表示一个有意义的字符串，比如(SIZE X + Y, X + Y,55)，表明定义SIZE是一个宏常量，其表达式为X + Y，MACRO的种别号是55。
又如(+,operators,60)表明加法运算符的种别号为60。 
  接下来进行语法分析。语法分析首先分析的是宏常量的表达式，在词法分析得到符号表的同时，找到了每一个宏常量对应表达式分别列在title和content数组中（表达式可以是一个常数，
也可以是一个复杂式），然后将每个表达式中的变量以及常数都用小写字母i替代，因为设计的文法当中默认用i表示操作数。例如SIZE + X / Y 变为i+i/i，然后用算符优先文法来规约这
个表达式，并判断是否合法。 
  语法分析结束后，最后进行表达式计算。对所有经过语法分析并且合法的表达式进行计算。计算时首先将表达式转成逆波兰式，然后利用栈对其进行计算。每计算完一个表达式，就对
  result表进行更新，循环直至所有的宏计算完毕，所有表达式的计算结果都会显示在界面上。 表达式计算完毕之后，开始对源程序进行替换，替换过程与计算过程同步进行：扫描源程序，
  对宏常量以及相关表达式出现的地方，用计算得到的值进行替换，该值通过扫描符号表得到。替换完毕后，输出result.txt文件。
