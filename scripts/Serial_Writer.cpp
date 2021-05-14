/*----------------------
//Created  by Issaimaru

//Created at 2021-05-15
----------------------*/

#include <Serial_Writer.h>

Serial_Writer::Serial_Writer(PinName TxPin,PinName RxPin,int baudrate):_Serial(TxPin,RxPin,baudrate){} 
