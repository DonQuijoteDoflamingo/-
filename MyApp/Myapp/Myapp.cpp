#include <iostream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include<fstream>
#include <vector>
#include <deque>
#include <stack>
#include <stdio.h>
#define maxl 4
using namespace std;
typedef long long ll;
ofstream Question("Exercises.txt");
ofstream Answers("Answers.txt");
char Op[] = {'+', '-', '*', '/'};
                //+   -   *   /   %   (   )   =
char ss[8][8] ={{'>','>','<','<','<','<','>','>'},
                {'>','>','<','<','<','<','>','>'},
                {'>','>','>','>','>','<','>','>'},
                {'>','>','>','>','>','<','>','>'},
                {'>','>','>','>','>','<','>','>'},
                {'<','<','<','<','<','<','=','0'},
                {'>','>','>','>','>','0','>','>'},
                {'<','<','<','<','<','<','0','='}};
//大于号，弹出数字，进行计算，小于号，压入堆栈中
//把字符转化成相应的行和列
void initCmd(int argc,char *argv[],string &n,string &r,string &e,string &a)//初始化输入参数
{
		for(int i=1;i<argc;i++)
		{
			string s(argv[i]);
			if(s=="-n")
			{
				if(i+1<argc)
				{
					string str(argv[i+1]);
					if(str!="-n" && str!="-r" && str!="-e" && str!="-a")
					{
						n=str;
					}
					if(n=="")
					{
						cout<<"请输入-n的值！";
						return;
					}
				}
			}
			if(s=="-r")
			{
				if(i+1<argc)
				{
					string str(argv[i+1]);
					if(str!="-n" && str!="-r" && str!="-e" && str!="-a")
					{
						r=str;
					}
					if(r=="")
					{
						cout<<"请输入-r的值！";
						return;
					}
				}
			}
			if(s=="-e"){
				if(i+1<argc){
					string str(argv[i+1]);
					if(str!="-n" && str!="-r" && str!="-e" && str!="-a")
					{
						e=str;
					}
					if(e=="")
					{
						cout<<"请输入-e的值！";
						return;
					}
				}
			}
			if(s=="-a")
			{
				if(i+1<argc)
				{
					string str(argv[i+1]);
					if(str!="-n" && str!="-r" && str!="-e" && str!="-a")
					{
						a=str;
					}
					if(a=="")
					{
						cout<<"请输入-a的值！";
						return;
					}
				}
			}
		}
	}
 void create(int argc,char *argv[])
{
	string n,r,e,a;
	initCmd(argc,argv,n,r,e,a);
	int R=atoi(r.c_str());
	//首先随机生成算式中操作数的个数，其数量必须大于1 
	int length;
	do{
		length = rand()%maxl;
	}while(length < 2);
		bool div = false; //用来防止出现除0错误 
	int brack_cnt = 0; //记录未匹配的左括号个数 
	int num=0, op=0;
	for (int i = 1; i < length; i++) //循环生成算式 
	{
		if (div) //若此时需要生成的数字前的负号是'/'，则需要特判此次生成的数字不能为0 
		{
		div = false;
		do{
			num = rand()%R;
		}while(num == 0);
			Question<<num;
		}
		else
			Question<<rand()%R; //否则直接生成数字输出 
		int tmpcnt = brack_cnt;
		for (int j = 0; j < tmpcnt; j++) //若当前有未匹配的左括号，则对每一个未匹配的左括号，都有一定概率生成相应右括号。 
		{
			if ((rand()%5) > 2) //生成右括号概率为0.6
			{
				brack_cnt--;
				Question<<")";
			}
		}
		op = rand()%4; //生成运算符 
		Question<<Op[op];
		if (op == 3) //若生成了除号，则需要置相应标志位 
			div = true;
		if (!(rand()%3)) //以一定概率生成左括号，概率为1/3 
		{
			Question<<"(";
			brack_cnt++;
			Question<<rand()%R; //生成左括号后必须生成一个数字和运算符，不然可能出现(15)这样的错误 
			op = rand()%4;
			Question<<Op[op];
			if (op == 3)
				div = true;
		}
	}
	if (div) //生成最后一个数字，该数字后不需要跟运算符 
	{
		div = false;
		do{
			num = rand()%R;
		}while(num == 0);
			Question<<num;
	}
	else
		Question<<rand()%R;
	while(brack_cnt--) //补全右括号 
		Question<<")";
}
 void getRowCol (const char stackCh, const char ch, int& row, int& col)
{
    switch (stackCh)
    {
    case '+':
        row = 0;
        break;
    case '-':
        row = 1;
        break;
    case '*':
        row = 2;
        break;
    case '/':
        row = 3;
        break;
    case '%':
        row = 4;
        break;
    case '(':
        row = 5;
        break;
    case ')':
        row = 6;
        break;
    case '=':
        row = 7;
        break;
    }
    switch (ch)
    {
    case '+':
        col = 0;
        break;
    case '-':
        col = 1;
        break;
    case '*':
        col = 2;
        break;
    case '/':
        col = 3;
        break;
    case '%':
        col = 4;
        break;
    case '(':
        col = 5;
        break;
    case ')':
        col = 6;
        break;
    case '=':
        col = 7;
        break;
    }

}
//写入文件
void writeAnswer(vector<int>& iAnswer)
{
	
	 for(vector<int>::size_type ix = 0; ix != iAnswer.size(); ix++)
		{
			Answers<< iAnswer[ix]<<endl;
		}
}
void countAnswer(vector<int>& iAnswer, string& str)
{
    int temp = 0;
    int row = 0 ,col = 0;
    stack<int> iStk;            //定义数据堆
    stack<char> cStk;            //定义字符堆
    char stackCh = '\0';
    char ch = '\0';
    cStk.push('=');             //先把=压入字符堆中，以备匹配等号
    for (string::size_type ix = 0; ix != str.size(); ix++)
    {
        //如果是数字，先判断下一个字符是否也是数字，如果不是，就压入数据堆中
        if (str[ix] >= '0' && str[ix] <= '9')
        {
            temp = str[ix] - '0' + temp * 10;
            if (str[ix + 1] < '0' || str[ix + 1] > '9')
            {
                iStk.push(temp);
            }
        }
        else
        {
/*            iStk.push(temp);*/
            temp = 0;
            stackCh = cStk.top();       //弹出字符堆的堆顶字符
            ch = str[ix];                //读取字符串中的字符
            if (ch == '=' || ch == ')') //遇到 = 号，或者），先执行判断
            {
                ix--;
            }
            getRowCol(stackCh,ch,row,col); ////把字符转化成相应的行和列
            switch(ss[row][col])
            {
            case '=':  //遇到括号匹配，或者=号匹配，计算结束
                if ( ch == '=')
                {
                    iAnswer.push_back(iStk.top());
                    return ;
                }
                else
                {
                    cStk.pop();
                    ix++;
                }

                break;
            case '>':  //取出堆中的两个数据，执行计算
                {
                    int num2 = iStk.top();
                    iStk.pop();
                    int num1 = iStk.top();
                    iStk.pop();
                    stackCh = cStk.top();
                    cStk.pop();
                    if (ch != '=' && ch != ')')
                    {
                        cStk.push(ch);
                    }
                    switch(stackCh)
                    {
                    case '+':
                        num1 += num2;
                        break;
                    case '-':
                        num1 -= num2;
                        break;
                    case '*':
                        num1 *= num2;
                        break;
                    case '/':
                        num1 /= num2;
                        break;
                    case '%':
                        num1 %= num2;
                        break;
                    }
                    iStk.push(num1);
                    break;
                }
            case '<': //压入堆中
                cStk.push(ch);
                break;
            case '0': //表达式错误
                cout << "error arithmetic" << endl;
                return ;
            }
        }
    }
}
void countMath()

{
     ifstream ifs("Exercises.txt");
	if (!ifs.is_open())
    {
        ifs.clear();
        fstream fs("Exercises.txt",ios_base::out); //如果打开失败就创建文件Exercises.txt
        fs.close();
        ifs.open("Exercises.txt");
    }
    string temp;
    deque<string> sDeq;
    while (!ifs.eof())
    {
        getline(ifs,temp);                //按行读取数据，存储到双端队列中
        sDeq.push_back(temp);
    }
    ifs.close();
    vector<int> iAnswer;
    for (deque<string>::size_type ix = 0; ix != sDeq.size(); ix++)
    {
        string str;
        str = sDeq.front();
        sDeq.pop_front();
        ix--;
        //计算每一行的表达式
        countAnswer(iAnswer,str);
    }
    writeAnswer(iAnswer);
}
int main(int argc,char *argv[])
{
	string n,r,e,a;
	initCmd(argc,argv,n,r,e,a);
	int N=atoi(n.c_str());//获得需要生成算式的个数
	srand(time(0));
	for (int i = 0; i < N; i++)
	{
		
		while(N--)
		{
			create(argc,argv);
			Question<<"="<<endl;			
		}
		countMath();
	}
	
	cout<<"题目生成完毕";
}