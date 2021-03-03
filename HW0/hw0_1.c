//DSA Homework0-1
//Binary Algorithm for Greatest Common Divisor
#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#define MAX 257

void read_from_string(int* big, char* str);
void big_print(int *big);
void big_sub(int *a, int *b);
int big_compare(int *a, int *b);
void big_multi(int* a, int b);
void big_divide(int* a, int b);
void big_swap(int* a, int* b);

int main()
{
    int ans = 1;
    char s1[MAX], s2[MAX];
	int a[MAX] = {0};  // 數字陣列num1 
	int b[MAX] = {0};  // 數字陣列num2 
    int n[MAX] = {0};  //較小大數
    int m[MAX] = {0};  //較大大數
    int zero[MAX] = {0};  //0
    printf("輸入兩整數：");
	scanf("%s %s", s1, s2);  //以字串讀取大數
    read_from_string(a,s1);  //將大數字串轉為整數陣列(低位數排至高位數)
    read_from_string(b,s2);

    if(big_compare(a,b)>0){
        memcpy(n,b,sizeof(b));  //deep copy
        memcpy(m,a,sizeof(a));  
    }
    else{
        memcpy(n,a,sizeof(a));  
        memcpy(m,b,sizeof(b));  
    }
    
    while (big_compare(n,zero)!=0 && big_compare(m,zero)!=0){
        if((n[0] % 2 == 0) && (m[0] % 2 ==0)){
            ans*=2;
            big_divide(n,2);
            big_divide(m,2);
        }
        else if(n[0] % 2 == 0)
            big_divide(n,2);
        else if(m[0] % 2 == 0)
            big_divide(m,2);
        if(big_compare(n,m)>0)
            big_swap(n,m);
        big_sub(m,n);
    }
    big_multi(n,ans);
    big_print(n);
    return 0;
}

void read_from_string(int* big, char* str)
{
    int i=0, len=strlen(str);
    for(i=len-1; i>=0; --i)
        big[len - i - 1] = str[i] - '0';
}
void big_print(int *big)
{
    int i=MAX-1;
    for(i=MAX-1;i>=0 && big[i]==0; --i);
    while(i>=0) printf("%d", big[i]), --i;
}
void big_sub(int *a, int *b)
{
    int rst[MAX]={0};
    int i, borrow=0;
    for(i=0; i<MAX; ++i) {
        rst[i] = a[i]-b[i]-borrow; // 扣上一次借位
        if(rst[i]<0) 
            borrow=1, rst[i]+=10; // 需借位
        else 
            borrow=0; // 無需借位
    }
    memcpy(a,rst,sizeof(rst));
}
int big_compare(int *a, int *b)
{
    int i=MAX-1;
    while(i>0 && a[i]==b[i]) --i;
    return a[i]-b[i];
    //a > b: +; a == b: 0; a < b: -;
}

void big_multi(int* a, int b)  //b為整數乘數
{ 
    int rst[MAX]={0};
    int i, tmp, carry = 0; 
    for(i = 0; i <MAX; i++) { 
        tmp = a[i] * b + carry; 
        rst[i] = tmp % 10;    
        carry = tmp / 10; 
    }
    memcpy(a,rst,sizeof(rst));
} 
void big_divide(int* a, int b)  //b為除數 
{  
    int rst[MAX]={0};
    int i, tmp, remain = 0; 
    for(i = MAX-1; i >= 0; i--) { 
        tmp = a[i] + remain; 
        rst[i] = tmp / b; 
        remain = (tmp % b) * 10; 
    }
    memcpy(a,rst,sizeof(rst));
}
void big_swap(int* a, int* b)
{
    int temp[MAX] = {0};
    for (int i=0;i<MAX;i++) {
		temp[i]=a[i];
		a[i]=b[i];
		b[i]=temp[i];
	}
}