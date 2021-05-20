/*----------------------

//Created  by Issaimaru

//Created at 2021-05-21(Update)

----------------------*/

#ifndef SERIAL_WRITER
#define SERIAL_WRITER
#include <mbed.h>
#include <iostream>

class Serial_Writer
{
public:
    Serial_Writer(PinName txPin,PinName rxPin,int baudrate);
    
    int readable();
    void attach(void (*fptr)());
    
    template<typename T,std::size_t N>void write(T (&send)[N]){
        writer<T,N>::write(send,_Serial);
    }
    
    template <typename T,std::size_t N>int receive(T (&get)[N]){
        return receiver<T,N>::receive(get,_Serial);
    }
    
    Serial _Serial;
    
private:
//--------------------------------------write--------------------------------------//

    //下記の型以外の型のときの処理
    template <typename T,std::size_t N>
    struct writer{
        static void write(T (&send)[N],Serial &_Serial){
            const int byte=sizeof(send);
            char buffer[byte+2];
            for (int i=1,k=0;i<=byte;k++){
                for(int byte_shift=sizeof(send[0])-1;byte_shift>=0;i++,byte_shift--)buffer[i]=(send[k]>>(8*byte_shift))&0xFF;
            }
            buffer[0]='[';
            buffer[byte+1]=']';
            for (int p=0;p<sizeof(buffer);p++)_Serial.putc(buffer[p]);
        }
    };
    
    //doubleのときの処理
    template<std::size_t N>
    struct writer<double,N>{
        static void write(double (&send)[N],Serial &_Serial){
            const int arrays_num=sizeof(send)/sizeof(send[0]);
            long long send_long[arrays_num];
            for(int j=0;j<arrays_num;j++)send_long[j]=send[j]*10000000;//小数点以下7桁、最大11桁(指数部4095)まで対応(doubleの有効桁数を参照)
            writer<long long,N>::write(send_long,_Serial);
        }
    };
    
    //floatのときの処理
    template<std::size_t N>
    struct writer<float,N>{
        static void write(float (&send)[N],Serial &_Serial){
            const int arrays_num=sizeof(send)/sizeof(send[0]);
            long long send_long[arrays_num];
            for(int j=0;j<arrays_num;j++)send_long[j]=send[j]*10000000;//小数点以下7桁、最大10桁(指数部255)まで対応(floatの有効桁数を参照)
            writer<long long,N>::write(send_long,_Serial);
        }
    };
    
    
//--------------------------------------end--------------------------------------//

//------------------------------------receive------------------------------------//
    
    //下記の型以外の型のときの処理
    template <typename T,std::size_t N>
    struct receiver{
        static int receive(T (&get)[N],Serial &_Serial){
            const int byte=sizeof(get);
            const int byte_0=sizeof(get[0]);
            char buffer[byte+2];
            for(int i=0;i<sizeof(buffer);i++){
                buffer[i]=_Serial.getc();
                if(buffer[0]!='[')return -1;
            }
            if(buffer[byte+1]==']'){
                for (int j=0;j<(byte/byte_0);j++)get[j]=0x0;
                for (int p=1,k=0;p<=byte;k++){
                    for (int byte_shift=byte_0-1;byte_shift>=0;p++,byte_shift--)get[k]|=buffer[p]<<(8*byte_shift);
                }
                return 0;//正常終了
            }else return -1;//異常終了1(正しく受信できていない)
        }
    };
    
    //doubleのときの処理
    template <std::size_t N>
    struct receiver<double,N>{
        static int receive(double (&get)[N],Serial &_Serial){
            long long arrays[N];
            int _return=receiver<long long,N>::receive(arrays,_Serial);
            for (int l=0;l<sizeof(arrays)/sizeof(arrays[0]);l++)get[l]=double(arrays[l])/10000000.0;
            return _return;
        }
    };
    
    //floatのときの処理
    template <std::size_t N>
    struct receiver<float,N>{
        static int receive(float (&get)[N],Serial &_Serial){
            long long arrays[N];
            int _return=receiver<long long,N>::receive(arrays,_Serial);
            for (int l=0;l<sizeof(arrays)/sizeof(arrays[0]);l++)get[l]=double(arrays[l])/10000000.0;
            return _return;
        }
    };
};

//------------------------------------end------------------------------------//


#endif