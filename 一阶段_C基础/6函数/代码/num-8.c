#include <stdio.h>
char  *InverseString(char *pstr); 
int  main()
    {
	    char str[6]={'a', 'b', 'c', 'd', 'e', '\0'} ;
   	    printf("%s\n",InverseString(str));
     }                                   
    char  * InverseString(char *pstr)       //定义一个指针函数  
       { 
          static char text[128];
             int len=0;
	     int i;
             while( *(pstr+len) != '\0')   	 //求字符串的长度
             len++;
for(i= len-1;  i>=0;  i-- )
  text[len-i-1] = *(pstr+i);		 //实现字符串的位置倒序；
  text[len]='\0';
  return(text);	
 }

