/*----------------------

//Created  by Issaimaru

//Created at 2021-05-15

----------------------*/

#include "Serial_Writer.h"
Serial_Writer TWE(D5,D4,115200);//Serial_Writer オブジェクト名(TxPin RxPin baudrate); シリアル通信に用いるピンとbaudrateの設定
int main(void)
{
    //オブジェクト名.attach(関数名); 受信割り込み
    while(true) {
        char consts[3]={'A','B','C'};//送信側と受信側の型、配列数を一致させてください
        TWE.write(consts);//オブジェクト名.write(送る配列); 送信
        wait_ms(5);//データの品質を保つため送信側に適度な待ち時間を入れてください
        
        //int n=オブジェクト名.receive(受信する配列); 正しくデータが配列に入っている時戻り値が0になります
        //int l=オブジェクト名.readable(); 受信データがあるとき戻り値が1になります(受信割り込みが使えるマイコンの場合受信割り込みを使ったほうが良いです)
    }
}
