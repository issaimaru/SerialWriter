/*----------------------

//Created  by Issaimaru

//Created at 2021-05-21(Update)

----------------------*/

#include <Serial_Writer.h>

Serial_Writer::Serial_Writer(PinName txPin,PinName rxPin,int baudrate):_Serial(txPin,rxPin,baudrate){} 

int Serial_Writer::readable(){
    return _Serial.readable();
}

void Serial_Writer::attach(void (*fptr)()){
    _Serial.attach(fptr,Serial::RxIrq);
}


