#include <stdio.h>
char *q[3] = { "Sun rises in?",
                                "Sun sets in?",
                                "Earth shape is?" };

char *a[3][4] = { {"a) east" , "b) west", "c) north", "d) south"}, 
                    {"a) east" , "b) west", "c) north", "d) south"},
                     {"a) sqaure", "b) giode", "c) sphere", "d) triangle"} };
char* Q(int i){
    return q[i];
}
void A(int i){
    printf("%s\n%s\n%s\n%s\n",a[i][0],a[i][1], a[i][2],a[i][3]);
}
int main(){
    printf("%s\n", Q(0));
    A(0);
}