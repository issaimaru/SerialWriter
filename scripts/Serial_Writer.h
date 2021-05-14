/*----------------------
//Created  by Issaimaru

//Created at 2021-05-15
----------------------*/

#ifndef SERIAL_WRITER
#define SERIAL_WRITER
#include <mbed.h>
#include <iostream>

class Serial_Writer
{
public:
    Serial_Writer(PinName TxPin,PinName RxPin,int baudrate);
    
    template<typename T,std::size_t N>void write(T (&send)[N],int delay){
        writer<T,N>::write(send,delay,_Serial);
    }
    
    template <typename R,std::size_t S>int receive(R (&get)[S]){
        return receiver<R,S>::receive(get,_Serial);
    }
    
    Serial _Serial;
    
private:
//--------------------------------------write--------------------------------------//

    //下記の型以外の型のときの処理
    template <typename T,std::size_t N>
    struct writer{
        static void write(T (&send)[N],int delay,Serial &_Serial){
            const int num=sizeof(send);
            char buffer[num+2];
            for (int i=1,k=0;i<=num;k++){
                for(int _bitNum=sizeof(send[0])-1;_bitNum>=0;i++,_bitNum--)buffer[i]=(send[k]>>(8*_bitNum))&0xFF;
            }
            buffer[0]='[';
            buffer[num+1]=']';
            for (int p=0;p<sizeof(buffer);p++)_Serial.putc(buffer[p]);
            wait_ms(delay);
        }
    };
    
    //doubleのときの処理
    template<std::size_t N>
    struct writer<double,N>{
        static void write(double (&send)[N],int delay,Serial &_Serial){
            const int numer=sizeof(send)/sizeof(send[0]);
            long long send_c[numer];
            for(int _n=0;_n<numer;_n++)send_c[_n]=send[_n]*10000000;//小数点以下7桁、最大11桁まで対応(doubleの有効桁数を参照)
            writer<long long,N>::write(send_c,delay,_Serial);
        }
    };
    
    //floatのときの処理
    template<std::size_t N>
    struct writer<float,N>{
        static void write(float (&send)[N],int delay,Serial &_Serial){
            const int numer=sizeof(send)/sizeof(send[0]);
            long long send_c[numer];
            for(int _n=0;_n<numer;_n++)send_c[_n]=send[_n]*10000000;//小数点以下7桁、最大10桁まで対応(floatの有効桁数を参照)
            writer<long long,N>::write(send_c,delay,_Serial);
        }
    };
    
//--------------------------------------end--------------------------------------//

//------------------------------------receive------------------------------------//
    
    //下記の型以外の型のときの処理
    template <typename R,std::size_t S>
    struct receiver{
        static int receive(R (&get)[S],Serial &_Serial){
            const int num=sizeof(get);
            const int num_0=sizeof(get[0]);
            char buffer[num+2];
            if (_Serial.readable()){
                for(int i=0;i<sizeof(buffer);i++){
                    buffer[i]=_Serial.getc();
                    if(buffer[0]!='[')return -1;
                }
                if(buffer[num+1]==']'){
                    for (int s=0;s<(num/num_0);s++)get[s]=0x0;
                    for (int p=1,k=0;p<=num;k++){
                        for (int _byte=num_0-1;_byte>=0;p++,_byte--)get[k]|=buffer[p]<<(8*_byte);
                    }
                    return 0;//正常終了
                }else return -1;//異常終了1(正しく受信できていない)
            }else return -2;//異常終了2(受信するものがない)
        }
    };
    
    //doubleのときの処理
    template <std::size_t S>
    struct receiver<double,S>{
        static int receive(double (&get)[S],Serial &_Serial){
            long long geter[S];
            int _return=receiver<long long,S>::receive(geter,_Serial);
            for (int _g_=0;_g_<sizeof(geter)/sizeof(geter[0]);_g_++)get[_g_]=double(geter[_g_])/10000000.0;
            return _return;
        }
    };
    
    //floatのときの処理
    template <std::size_t S>
    struct receiver<float,S>{
        static int receive(float (&get)[S],Serial &_Serial){
            long long geter[S];
            int _return=receiver<long long,S>::receive(geter,_Serial);
            for (int _g_=0;_g_<sizeof(geter)/sizeof(geter[0]);_g_++)get[_g_]=double(geter[_g_])/10000000.0;
            return _return;
        }
    };
};

//------------------------------------end------------------------------------//


#endif