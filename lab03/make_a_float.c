#include <stdio.h>
#include <stdlib.h>
int main (int argc ,char * argv[]){
    int s = 0;
    scanf("%d",&s);
    int res = 0;
    int t = 0;
    t = s;
    //0 times == 1
    if (t == 0){
        res = 1;
    }
    //1 times == 2
    else if (t == 1){
        res = 2;
    }
    // mult 2 at n - 1 times
    else {
        int buff = 2;
        while((t-1) > 0){
            buff = buff * 2;
            t--;
        }
        res = buff;
    }
    printf("res = %d" ,res);
    //return a value
    return res;
}
