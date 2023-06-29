#include "shudu.cpp"
#include "gtest/gtest.h"

//测试输入
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

TEST(Input, s)
{
    char *temp[]={"","-s","test.txt"};
    Input input(3, temp);
    input.selectJudgement();
    EXPECT_STREQ(input.file_name, "test.txt");
    EXPECT_EQ(input.select, 's');
}

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

//测试初始化过程
TEST(ArrayInit, Num1)
{
    Shudu shudu;
    shudu.arrayInit(1);
    ASSERT_TRUE(shudu.out_array_ != NULL); 
}

TEST(ArrayInit, Num0)
{
    Shudu shudu;
    shudu.arrayInit(0);
    ASSERT_TRUE(shudu.out_array_ != NULL); 
}

//


int main(int argc, char* argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
