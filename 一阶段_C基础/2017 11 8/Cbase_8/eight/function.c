#include <stdio.h>

//声明
void HelloWorld(void);

//定义
void HelloWorld(void)
{
    printf("Hello World\n");

    return;
}

int main()
{
    //调用
    HelloWorld();
    HelloWorld();

    return 0;
}

