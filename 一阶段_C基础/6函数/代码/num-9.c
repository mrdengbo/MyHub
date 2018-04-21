#include <stdio.h>
     int test(int a,int b,int (*pFunt)(int m,int n)); 
     int Plus(int a,int b);              //函数说明       
     int Minus(int , int );              //函数说明，缺省形式参数名称  
     int  main(void)
     {
	    int x=5, y=8;
	    int  (* pFunc)(int a,int b);     //说明一个名称为pFunc的函数指针
            pFunc=Plus;              //把函数Plus的地址赋给函数指针pFunc；
	    printf("%d\n", (*pFunc)(x,y) );
	    pFunc=Minus;        //把函数Minus的地址赋给函数指针 pFunc；
	
	   printf("%d\n", (*pFunc)(x,y) );
           printf("%d\n", test(15,5,Plus) ); //把函数Plus作为实参调用test
           printf("%d\n", test(15,5,Minus) );//把函数Minus作为实参调用test
      }

int Plus(int a,int b)               //简单的加法函数
        {	    return(a+b);     }

int Minus(int a,int b)            //简单的减法函数
        {	return(a-b);        }

int test(int a,int b, int (*pFunt)(int m,int n)) //简单的测试函数指针的函数
       { 	return( (*pFunt)(a,b));         }


