#include"protocol.h"
#include<string.h>
#include<stdlib.h>


PDU *mkPDU(uint uiMsgLen)
{
    uint uiPDULen = sizeof(PDU) + uiMsgLen;
    PDU * pdu = (PDU *)malloc(uiPDULen);
    if(pdu == nullptr){
        exit(1);
    }
    memset(pdu,0,uiPDULen);
    pdu ->uiPDULen = uiPDULen;
    pdu ->uiMsgLen = uiMsgLen;
    return pdu;
}
