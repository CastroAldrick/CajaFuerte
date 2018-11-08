
#include <stdio.h>


void add(int *p){
    for (int i = 0; i <= 4; i++){
        int x = *(p+i);
        printf("%d", x);
    }
    return;
}

int main(){
    int array[] = {2,4,9,3,0};
    add(array);
}

/*
int main()
{
    printf("Hello World");

    return 0;
}*/
