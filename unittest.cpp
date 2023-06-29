#include "shudu.cpp"
#include "gtest/gtest.h"

bool checkAnswer(char board[10][10]);
void intoBoard(char* array, char board[10][10]);
int countV(char *array);

/*测试输入*/

//测试-c输入
TEST(Input, c1)
{
    char *temp[]={"","-c","1"};
    Input input(3, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 1);
    EXPECT_EQ(input.select, 'c');
}

TEST(Input, c1000000)
{
    char *temp[]={"","-c","1000000"};
    Input input(3, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 1000000);
    EXPECT_EQ(input.select, 'c');
}

//测试-s输入
TEST(Input, s)
{
    char *temp[]={"","-s","test.txt"};
    Input input(3, temp);
    input.selectJudgement();
    EXPECT_STREQ(input.file_name, "test.txt");
    EXPECT_EQ(input.select, 's');
}

//测试-n输入
TEST(Input, n1)
{
    char *temp[]={"","-n","1"};
    Input input(3, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 1);
    EXPECT_EQ(input.select, 'n');
}


TEST(Input, n10000)
{
    char *temp[]={"","-n","10000"};
    Input input(3, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 10000);
    EXPECT_EQ(input.select, 'n');
}

//测试-m输入
TEST(Input, n1_m1)
{
    char *temp[]={"","-n","1","-m","1"};
    Input input(5, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 1);
    EXPECT_EQ(input.m_, 1);
    EXPECT_EQ(input.select, 'm');
}

TEST(Input, n10000_m3)
{
    char *temp[]={"","-n","10000","-m","3"};
    Input input(5, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 10000);
    EXPECT_EQ(input.m_, 3);
    EXPECT_EQ(input.select, 'm');
}

//测试-r输入
TEST(Input, n1_r20_55)
{
    char *temp[]={"","-n","1","-r","20~55"};
    Input input(5, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 1);
    EXPECT_STREQ(input.r_, "20~55");
    EXPECT_EQ(input.select, 'r');
}

TEST(Input, n10000_r20_55)
{
    char *temp[]={"","-n","10000","-r","20~55"};
    Input input(5, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 10000);
    EXPECT_STREQ(input.r_, "20~55");
    EXPECT_EQ(input.select, 'r');
}

//测试-u输入
TEST(Input, n1_u)
{
    char *temp[]={"","-n","1","-u"};
    Input input(4, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 1);
    EXPECT_EQ(input.select, 'u');
}

TEST(Input, n10000_u)
{
    char *temp[]={"","-n","10000","-u"};
    Input input(4, temp);
    input.selectJudgement();
    EXPECT_EQ(input.map_num, 10000);
    EXPECT_EQ(input.select, 'u');
}

/*测试功能*/

//测试初始化功能
TEST(Shudu, Init_20)
{
    Shudu shudu;
    shudu.arrayInit(20);
    ASSERT_TRUE(shudu.out_array_ != NULL); 
    EXPECT_EQ(shudu.output_num_, 20);
}

//测试终盘生成
TEST(Shudu, Generate_Result)
{
    Shudu shudu;
    shudu.arrayInit(1);
    shudu.strategyC(1,2);
    ASSERT_TRUE(checkAnswer(shudu.board_)); 
}

//测试数独求解
TEST(Shudu, Generate_Solution)
{
    Shudu shudu;
    shudu.arrayInit(1);
    shudu.strategyS("test.txt");
    ASSERT_TRUE(checkAnswer(shudu.board_)); 
}

//测试数独生成
TEST(Shudu, Generate_Game)
{
    Shudu shudu;
    shudu.arrayInit(1);
    shudu.strategyC(1, 2);
    shudu.generateN();
    ASSERT_TRUE(checkAnswer(shudu.board_)); 
}

//测试-m参数
TEST(Shudu, Test_m1)
{
    Shudu shudu;
    shudu.arrayInit(1);
    shudu.strategyC(1, 2);
    shudu.generateM(1);
    int count=countV(shudu.out_array_);
    EXPECT_EQ(count, 27); 
}

TEST(Shudu, Test_m2)
{
    Shudu shudu;
    shudu.arrayInit(1);
    shudu.strategyC(1, 2);
    shudu.generateM(2);
    int count=countV(shudu.out_array_);
    EXPECT_EQ(count, 45); 
}

TEST(Shudu, Test_m3)
{
    Shudu shudu;
    shudu.arrayInit(1);
    shudu.strategyC(1, 2);
    shudu.generateM(3);
    int count=countV(shudu.out_array_);
    EXPECT_EQ(count, 54); 
}

//测试-r参数
TEST(Shudu, Test_r)
{
    Shudu shudu;
    shudu.arrayInit(1);
    shudu.strategyC(1, 2);
    shudu.generateR("20~25");
    int count=countV(shudu.out_array_);
    ASSERT_TRUE(count>=20 && count<=25); 
}

//测试-u参数
TEST(Shudu, Test_u)
{
    Shudu shudu;
    shudu.arrayInit(1);
    shudu.strategyC(1, 2);
    shudu.generateR("20~46");
    shudu.mapSolveUnique();
    ASSERT_TRUE(checkAnswer(shudu.board_));
}

int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

/*辅助函数*/

//检验数独是否有效
bool checkAnswer(char board[10][10])
{
    int row[10][10]{0};
    int cow[10][10]{0};
    int bBox[10][10][10]{0};
    int index(0);
    for (int i=1; i<10; i++){
        for(int j=1; j<10; j++){
            if (board[i][j] != '$'){
                index = board[i][j]-'0';
                row[i][index]++;
                cow[j][index]++;
                bBox[((i - 1) / 3) * 3 + 1][((j - 1) / 3) * 3 + 1][index]++;
                if (row[i][index]==2 || cow[j][index]==2 || bBox[((i - 1) / 3) * 3 + 1][((j - 1) / 3) * 3 + 1][index]==2)
                    return false;
            }
        }
    }
    return true;
}

//计算挖空总数
int countV(char *array)
{
    int count=0;
    for(int i=0;i<168;i++)
    {
        if(array[i]=='$')
        {
            count++;
        }
    }
    //cout<<"Count:"<<count<<endl;
    return count;
}

void intoBoard(char* array, char board[10][10])
{
    int col=1, row=1;
    for(int i=0;i<168;i++)
    {
        if(array[i]!=' '||array[i]!='\n')
        {
            board[row][col]=array[i];
            col++;
            if(col==9)
            {
                col=1;
                row++;
                if(row==10)break;
            }
        }
    }

}
