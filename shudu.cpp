#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <regex>
#include <sstream>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <SDKDDKVer.h>
using namespace std;
#define NUMBER 9  // 1~9的9个数

class Input
{
private:
	int argc_;
	char** argv_;

public:
	char select; //命令行参数
	char* file_name; //-s 要求解的数独的文件名
	int map_num; //-c 生成的数独终盘的数量
	int m_; //生成数独的难度等级1-3
	char *r_;//生成数独的挖空数量范围
	Input(int argc, char** argv);
	void selectJudgement(); //判断命令行参数的函数
};

class Shudu
{
public:
	int num_;				   // 已生成数独终局个数
	int output_num_;		   // 要求生成数独终局的个数
	char* out_array_;		   // 储存要输出的终局
	int out_array_pointer_;

	bool num_row[10][10];      // [num][i]：数字num在第i行是否可填
	bool num_col[10][10];	   // [num][j]：数字num在第j列是否可填
	bool num_box[10][10][10];  // [num][row][col]：数字num在[row][col]格所在的九宫格中是否可填
	char board_[10][10];

	Shudu();
	void arrayInit(int Out_Num); //存数独的数组初始化
	void strategyC(int i, int j); //-c
	void strategyS(char *file_name); //-s
	bool shuduSolve(int i, int j); //求解一个数独问题
	void shuchu(); //输出数独终局或问题到txt文件
	void shuchuResult(); //输出数独结果到txt文件
	bool readShudu(fstream &read_file);  //读取数独问题
	void intoOutArray(); //将数独地图放到数组中
	void generateN(); //-n 生成游戏
	void generateM(int m); //-m
	void generateR(const char *r); //-r
	bool isSolve(int i, int j); //判断数独是否能解
	void mapSolveUnique(); //遍历每个数独盘，进行解的唯一化
	bool SolveUnique(); //判断数独盘的解是否唯一
};

Input::Input(int argc, char** argv)
{
	this->argc_ = argc;
	this->argv_ = argv;
	this->select = '\0';
	this->map_num = 0;
	this->m_ = 0;
	this->r_ = NULL;
	this->file_name = NULL;
}

void Input::selectJudgement()
{
	if (this->argc_ !=3 && this->argc_ !=4 && this->argc_ !=5 )
	{
		cout << "参数数量错误" << endl;
		exit(-1);
	}
	if (this->argv_[1][0] == '-' && this->argv_[1][1] == 'c'&& strlen(this->argv_[1]) == 2 )
	{ //-c
		this->select = 'c';
		regex reg_pattern("^\\+?[1-9][0-9]*$");  // 验证非零的正整数
		if (regex_match(this->argv_[2], reg_pattern) && atoi(this->argv_[2])>=1 && atoi(this->argv_[2])<=1000000)
			this->map_num = atoi(this->argv_[2]);
		else{
			cout << "-c 命令参数错误" << endl;
			exit(-1);
		}
	}

	else if (this->argv_[1][0] == '-' && this->argv_[1][1] == 's' && strlen(this->argv_[1]) == 2){ //-s
		this->select = 's';
		this->file_name = this->argv_[2];
	}

	else if(this->argv_[1][0] == '-' && this->argv_[1][1] == 'n' && strlen(this->argv_[1]) == 2){
		if(this->argc_==3){  //-n
			this->select = 'n';
			regex reg_pattern("^\\+?[1-9][0-9]*$");  // 验证非零的正整数
			if (regex_match(this->argv_[2], reg_pattern) && atoi(this->argv_[2])>=1 && atoi(this->argv_[2])<=10000)
				this->map_num = atoi(this->argv_[2]);
			else{
				cout << "-n 命令参数错误" << endl;
				exit(-1);
			}
		}
		else if(this->argc_==4 && this->argv_[3][0] == '-' && this->argv_[3][1] == 'u' && strlen(this->argv_[3]) == 2){  //-n -u
			this->select = 'u';
			regex reg_pattern("^\\+?[1-9][0-9]*$");  // 验证非零的正整数
			if (regex_match(this->argv_[2], reg_pattern) && atoi(this->argv_[2])>=1 && atoi(this->argv_[2])<=10000)
				this->map_num = atoi(this->argv_[2]);
			else{
				cout << "-n 命令参数错误" << endl;
				exit(-1);
			}
		}
		else if(this->argv_[3][0] == '-' && this->argv_[3][1] == 'm' && strlen(this->argv_[3]) == 2){ //-n -m
			this->select = 'm';
			regex reg_pattern("^\\+?[1-9][0-9]*$");  // 验证非零的正整数
			if (regex_match(this->argv_[2], reg_pattern) && atoi(this->argv_[2])>=1 && atoi(this->argv_[2])<=10000
				&&regex_match(this->argv_[4], reg_pattern) && atoi(this->argv_[4])>=1 && atoi(this->argv_[4])<=3){
				this->map_num = atoi(this->argv_[2]);
				this->m_  = atoi(this->argv_[4]);
			}
			else{
				cout << "-n 命令参数错误" << endl;
				exit(-1);
			}
		}
		else if(this->argv_[3][0] == '-' && this->argv_[3][1] == 'r' && strlen(this->argv_[3]) == 2){ //-n -r
			this->select = 'r';
			regex reg_pattern("^\\+?[1-9][0-9]*$");  // 验证非零的正整数
			if (regex_match(this->argv_[2], reg_pattern) && atoi(this->argv_[2])>=1 && atoi(this->argv_[2])<=10000){
				this->map_num = atoi(this->argv_[2]);
				this->r_ = this->argv_[4];
			}
			else{
				cout << "-n 命令参数错误" << endl;
				exit(-1);
			}
		}
		else{
			cout << "命令有误！" << endl;
			exit(-1);
		}
	}
	else{
		cout << "命令有误1！" << endl;
		exit(-1);
	}
}

Shudu::Shudu()
{
	memset(board_, '$', sizeof(board_));
	memset(num_row, true, sizeof(num_row));
	memset(num_col, true, sizeof(num_col));
	memset(num_box, true, sizeof(num_box));

	this->board_[1][1] = 4 + '0'; 
	this->num_row[4][1] = false;
	this->num_col[4][1] = false;
	this->num_box[4][1][1] = false;
	this->num_ = 0;
	this->output_num_ = 0;
	this->out_array_pointer_ = 0;
	this->out_array_ = NULL;       // 没有new对象
}

void Shudu::arrayInit(int Output_Num) //所有数独对应的数组进行初始化
{
	this->output_num_ = Output_Num;
	this->out_array_ = new char[163 * Output_Num + 5]; //163是每个数独终局需要的字符数量：81*2+1
	memset(out_array_, '\0', sizeof(out_array_));
}

void Shudu::intoOutArray() //将一个数独终盘放进数组
{
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 1; j <= 8; j++)
		{
			this->out_array_[this->out_array_pointer_++] = this->board_[i][j];
			this->out_array_[this->out_array_pointer_++] = ' ';
		}
		this->out_array_[this->out_array_pointer_++] = this->board_[i][9];
		this->out_array_[this->out_array_pointer_++] = '\n';
	}
	this->out_array_[this->out_array_pointer_++] = '\n';
}

void Shudu::strategyC(int i, int j) //对应 -c 递归生成数独终盘
{
	if (this->num_ >= this->output_num_)
		return;

	if (i == 9 && j == 10){  // 当完成了一个新终局时
		this->num_++;
		this->intoOutArray();	
		return;
	}

	if (i != 9 && j == 10){ //当完成一行时
		i++;
		j = 1;
	}
	int row = ((i - 1) / 3) * 3 + 1;
	int col = ((j - 1) / 3) * 3 + 1;

	for (int num = 1; num <= NUMBER; num++)  // 填入数字num
	{
		if (this->num_ >= this->output_num_)
			return;

		if (this->num_row[num][i] == true //判断行是否能填
			&& this->num_col[num][j] == true //判断列是否能填
			&& this->num_box[num][row][col] == true) //判断所在的九宫格是否能填（用九宫格第一个数字代替整个区域）
		{
			this->board_[i][j] = num + '0';
			this->num_row[num][i] = false;
			this->num_col[num][j] = false;
			this->num_box[num][row][col] = false;

			strategyC(i, j + 1);

			this->num_row[num][i] = true;
			this->num_col[num][j] = true;
			this->num_box[num][row][col] = true;
		}

	}

	this->board_[i][j] = '$';  // 回溯时当前位
}

void Shudu::generateN() //生成数独游戏 -n
{
	this->out_array_pointer_ = 0;
	int number[4]; //默认游戏是每行挖空4个数,即挖空4*9=36个
	srand((unsigned)time(NULL)); //随机数：rand()生成不同的随机种子。
	for(int map = 0; map < this->output_num_; map++) //每张数独地图进行挖空
	{
		for (int i = 1; i <= 9; i++)
		{
			for(int k = 0; k < 4; k++) //生成四个不同的随机数
			{
				while(1){
					number[k] = rand() % 9 + 1;//生成1~9随机数
					int k1;
					for(k1 = 0; k1 < k; k1++)
					{
						if(number[k]==number[k1])
							break;
					}
					if(k1==k)
						break;
				}
			}
			this->out_array_[this->out_array_pointer_ + 2 * number[0] - 2] = '$';
			this->out_array_[this->out_array_pointer_ + 2 * number[1] - 2] = '$';
			this->out_array_[this->out_array_pointer_ + 2 * number[2] - 2] = '$';
			this->out_array_[this->out_array_pointer_ + 2 * number[3] - 2] = '$';
			this->out_array_pointer_ = this->out_array_pointer_ + 18; //将数组指针挪到下一行
		}
		this->out_array_pointer_++;
	}
}

void Shudu::generateM(int m) //生成数独游戏 -m
{
	this->out_array_pointer_ = 0;
	int *number;
	int rank; //记录每行的挖空数量
	if(m==1)
		rank = 3; //默认游戏是每行挖空3个数,即挖空3*9=27个
	else if(m==2)
		rank = 5; //默认游戏是每行挖空5个数,即挖空5*9=45个
	else
		rank = 6;  //默认游戏是每行挖空6个数,即挖空6*9=54个
	number = new int[rank]; 

	srand((unsigned)time(NULL)); //随机数：rand()生成不同的随机种子。
	for(int map = 0; map < this->output_num_; map++) //每张数独地图进行挖空
	{
		for (int i = 1; i <= 9; i++)
		{
			for(int k = 0; k < rank; k++) //生成rank个不同的随机数,并将该位置置为$
			{
				while(1){
					number[k] = rand() % 9 + 1;//生成1~9随机数
					int k1;
					for(k1 = 0; k1 < k; k1++)
					{
						if(number[k]==number[k1])
							break;
					}
					if(k1==k)
						break;
				}
				this->out_array_[this->out_array_pointer_ + 2 * number[k] - 2] = '$';
			}
			this->out_array_pointer_ = this->out_array_pointer_ + 18; //将数组指针挪到下一行
		}
		this->out_array_pointer_++;
	}
}

void Shudu::generateR(const char *r) //生成数独游戏 -r
{
	this->out_array_pointer_ = 0;
	int *number;
	int rank; //记录每行的挖空数量
	int min = (r[0] - '0') * 10 + r[1] - '0'; //挖空数量的最小值
	int max = (r[3] - '0') * 10 + r[4] - '0'; //挖空数量的最大值
	int minRank = min/9; //每行挖空最小值
	int maxRank = max/9; //每行挖空最大值
	srand((unsigned)time(NULL)); //随机数：rand()生成不同的随机种子。

	for(int map = 0; map < this->output_num_; map++) //每张数独地图进行挖空
	{
		int sum = 0;//记录每个终盘的挖空数量总和
		for (int i = 1; i <= 9; i++)
		{
			if(i==9 && sum < min) //最后一行要保证挖空数量总和>=最小值
				rank = min - sum;
			else
				rank = rand() % (maxRank-minRank+1) + minRank; //随机生成每行要挖空的数量
			sum = sum + rank;
			number = new int[rank]; 
			for(int k = 0; k < rank; k++) //生成rank个不同的随机数,并将该位置置为$
			{
				while(1){
					number[k] = rand() % 9 + 1;//生成1~9随机数
					int k1;
					for(k1 = 0; k1 < k; k1++)
					{
						if(number[k]==number[k1])
							break;
					}
					if(k1==k)
						break;
				}
				this->out_array_[this->out_array_pointer_ + 2 * number[k] - 2] = '$';
			}
			this->out_array_pointer_ = this->out_array_pointer_ + 18; //将数组指针挪到下一行
		}
		this->out_array_pointer_++;
	}
}


bool Shudu::readShudu(fstream &read_file) //读取数独问题
{
	string str_line;
	int num;
	memset(num_row, true, sizeof(num_row));
	memset(num_col, true, sizeof(num_col));
	memset(num_box, true, sizeof(num_box));
	for (int i = 1; i <= 9; i++)
	{
		if (!getline(read_file, str_line))
			return false;
		for (int k = 0, j = 1; k <= 16; k += 2, j++)
		{
			if(str_line[k]== '$')
				num = 0;
			else
				num = str_line[k] - '0';
			this->board_[i][j] = str_line[k];
			this->num_row[num][i] = false;
			this->num_col[num][j] = false;
			this->num_box[num][i][j] = false;
		}
	}
	getline(read_file, str_line);
	return true;
}

void Shudu::strategyS(char *file_name) //对应 -s 求解数独
{
	fstream read_file(file_name, ios::in);
	if (!read_file.is_open())
	{
		cout << "打开文件错误！" << endl;
		exit(-1);
	}

	while (readShudu(read_file))
	{
		if (shuduSolve(1, 1))
		{
			intoOutArray();
		}	
		else
			cout << "此局无解！" << endl;
	}

	read_file.close();
}

bool Shudu::shuduSolve(int i, int j) //利用递归解数独
{
	if (i == 9 && j == 10)
		return true;
	if (i != 9 && j == 10){
		i++;
		j = 1;
	}
	if (this->board_[i][j] != '$') //空格用$表示
		return shuduSolve(i, j + 1);

	int row = ((i - 1) / 3) * 3 + 1;
	int col = ((j - 1) / 3) * 3 + 1;

	for (int num = 1; num <= NUMBER; num++)
	{
		if (this->num_row[num][i] == true
			&& this->num_col[num][j] == true
			&& this->num_box[num][row][col] == true)
		{
			this->board_[i][j] = num + '0';
			this->num_row[num][i] = false;
			this->num_col[num][j] = false;
			this->num_box[num][row][col] = false;

			bool out_come = shuduSolve(i, j + 1);
			if (out_come)
				return true;

			this->num_row[num][i] = true;
			this->num_col[num][j] = true;
			this->num_box[num][row][col] = true;
		}
	}

	this->board_[i][j] = '$';
	return false;
}

bool Shudu::isSolve(int i, int j) //利用递归解数独，就是shuduSolve函数，不改变$值
{
	if (i == 9 && j == 10)
		return true;
	if (i != 9 && j == 10){
		i++;
		j = 1;
	}
	if (this->board_[i][j] != '$') //空格用$表示
		return shuduSolve(i, j + 1);

	int row = ((i - 1) / 3) * 3 + 1;
	int col = ((j - 1) / 3) * 3 + 1;

	for (int num = 1; num <= NUMBER; num++)
	{
		if (this->num_row[num][i] == true
			&& this->num_col[num][j] == true
			&& this->num_box[num][row][col] == true)
		{
			this->num_row[num][i] = false;
			this->num_col[num][j] = false;
			this->num_box[num][row][col] = false;

			bool out_come = shuduSolve(i, j + 1);
			if (out_come)
				return true;

			this->num_row[num][i] = true;
			this->num_col[num][j] = true;
			this->num_box[num][row][col] = true;
		}
	}
	return false;
}

void Shudu::mapSolveUnique(){ //遍历每张地图,对每个地图的解进行唯一化
	this->out_array_pointer_ = 0;
	int num, row, col;
	cout<<this->output_num_<<endl;
	for(int map = 0; map < this->output_num_; map++) //遍历每张地图,从数组中取出数独对应元素到board_[10][10]中，并更新判断条件
	{
		for (int i = 1; i <= 9; i++)
		{
			for (int j = 1; j <= 9; j++)
			{
				this->board_[i][j] = this->out_array_[this->out_array_pointer_++];
				if(this->board_[i][j] != '$'){
					num = board_[i][j] - '0';
					row = ((i - 1) / 3) * 3 + 1;
					col = ((j - 1) / 3) * 3 + 1;
					this->num_row[num][i] = false;
					this->num_col[num][j] = false;
					this->num_box[num][row][col] = false;
				}
				this->out_array_pointer_++;
			}
		}
		SolveUnique(); //对该数独盘唯一化，存到board_[i][j]
		this->out_array_pointer_ = this->out_array_pointer_ - 162;
		intoOutArray(); //将唯一化后的board_[i][j]存到输出数组中
	}
}

bool Shudu::SolveUnique() { //将数独盘化为唯一解
    int cnt;  // 解的个数
	int row, col;
    // 枚举所有空格
    for (int i = 1; i <= 9; i++) {
        for (int j = 1; j <= 9; j++) {
            if (board_[i][j] == '$') {  // 找到一个空格
                cnt = 0;
				row = ((i - 1) / 3) * 3 + 1;
				col = ((j - 1) / 3) * 3 + 1;
                // 尝试填入 1~9 的数字
                for (int k = 1; k <= 9; k++) {//模拟解数独过程，试探某个空有多解的情况,如果有多解，则不能挖空，把该位置的解固定为找到的第一个数
                    if (this->num_row[k][i] == true && this->num_col[k][j] == true && this->num_box[k][row][col] == true) { //可以填
						this->num_row[k][i] = false;
						this->num_col[k][j] = false;
						this->num_box[k][row][col] = false;
                        if (isSolve(i, j + 1)) { //找加入该位置填k时，数独是否有解
							if(cnt == 0)
								board_[i][j] = k + '0'; //把该位置的解固定为找到的第一个k
                            cnt++;  // 每找到一个解就+1
                            if (cnt > 1) { //多解
								this->num_row[k][i] = true; 
								this->num_col[k][j] = true;
								this->num_box[k][row][col] = true;
								break;
                            }
                        }
						else{ //无解
							this->num_row[k][i] = true;
							this->num_col[k][j] = true;
							this->num_box[k][row][col] = true;
						}
                    }
				}
				if(cnt == 1){
					board_[i][j] = '$';  // 如果该空只有一个解，可以挖空
				}
            }
        }
    }
	for (int i = 1; i <= 9; i++) { //最后为了下一个数独终盘需要，要判断条件全部变为true
        for (int j = 1; j <= 9; j++) {
			row = ((i - 1) / 3) * 3 + 1;
			col = ((j - 1) / 3) * 3 + 1;
			for (int k = 1; k <= 9; k++) {
				this->num_row[k][i] = true;
				this->num_col[k][j] = true;
				this->num_box[k][row][col] = true;
			}
		}
	}
	return true;
}

void Shudu::shuchu() //输出数组中对应的所有数独数独
{
	this->out_array_[this->out_array_pointer_] = '\0';  // 最后一个终局不用空行
	this->out_array_[this->out_array_pointer_ - 1] = '\0';
	this->out_array_[this->out_array_pointer_ - 2] = '\0';

	fstream file("shudu.txt", ios::out);
	file << this->out_array_;
	delete[] this->out_array_;
	this->out_array_ = NULL;
	file.close();
}

void Shudu::shuchuResult() //输出数组中对应的所有数独数独
{
	this->out_array_[this->out_array_pointer_] = '\0';  // 最后一个终局不用空行
	this->out_array_[this->out_array_pointer_ - 1] = '\0';
	this->out_array_[this->out_array_pointer_ - 2] = '\0';

	fstream file("result.txt", ios::out);
	file << this->out_array_;
	delete[] this->out_array_;
	this->out_array_ = NULL;
	file.close();
}

int main(int argc, char* argv[])
{
	Input input(argc, argv);
	Shudu shudu;
	input.selectJudgement();

	switch (input.select)
	{
	case 'c':
		shudu.arrayInit(input.map_num);
		shudu.strategyC(1, 2);
		shudu.shuchu();
		break;

	case 's':
		shudu.arrayInit(1000000);
		shudu.strategyS(input.file_name);
		shudu.shuchuResult();
		break;

	case 'n':
		shudu.arrayInit(input.map_num);
		shudu.strategyC(1, 2);
		shudu.generateN();
		shudu.shuchu();
		break;

	case 'u': //待实现
		shudu.arrayInit(input.map_num);
		shudu.strategyC(1, 2);
		shudu.generateR("20~46");
		shudu.mapSolveUnique();
		shudu.shuchu();
		break;

	case 'm': 
		shudu.arrayInit(input.map_num);
		shudu.strategyC(1, 2);
		shudu.generateM(input.m_);
		shudu.shuchu();
		break;

	case 'r':
		shudu.arrayInit(input.map_num);
		shudu.strategyC(1, 2);
		shudu.generateR(input.r_);
		shudu.shuchu();
		break;
	}
	cout << "生成成功！"<< endl;
	return 0;
}
