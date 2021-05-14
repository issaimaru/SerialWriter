/*----------------------
//Created  by Issaimaru

//Created at 2021-05-15
----------------------*/

#include "Serial_Writer.h"
Serial_Writer TWE(D5,D4,115200);
int main(void)
{
    while(true) {
        double consts[3]= {3.1415,2.71828,9.806}; //任意の型の配列
        TWE.write(consts,40);//インスタンス名.write(送りたい配列の名前,遅延(ms)) で送信
    }
}

/*
//受信側の例

int main(void)
{
    while(true) {
        double consts[3];//送信側と同じ型、同じ要素数の配列
        int n=TWE.receive(consts);//インスタンス名.receive(受信データを格納する配列の名前) 戻り値　0:正常に受信　1:データに欠損がある 2:受信データがない
        if(n==0) {
            printf("%3.2f %3.2f %3.2f\n\r",consts[0],consts[1],consts[2]);
        }
    }
}
*/