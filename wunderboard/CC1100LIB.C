#define BYTE unsigned char   
#define UINT8 unsigned char   
   
void halSpiWriteBurstReg(BYTE addr, BYTE *buffer, BYTE count);   
void halSpiReadBurstReg(BYTE addr, BYTE *buffer, BYTE count);   
void halSpiWriteReg(BYTE addr, BYTE value);   
void halSpiStrobe(BYTE strobe);   
BYTE halSpiReadReg(BYTE addr);   
BYTE halSpiReadStatus(BYTE addr);   
void halRfWriteRfSettings();   
void halRfSendPacket(BYTE *txBuffer, UINT8 size);   
BYTE halRfReceivePacket(BYTE *rxBuffer, UINT8 length);   
void POWER_UP_RESET_CC1100();   
BYTE spiGetRxTxStatus(void);   
BYTE SPI_write(BYTE value);   
BYTE SPI_read();   
#define WRITE_BURST     0x40   
#define READ_SINGLE     0x80   
#define READ_BURST      0xC0   
   
//----------------------------------------------------------   
void Dly1mS(unsigned int l)    
{   
    int i;   
    while(l--)  for(i=0;i<54;i++);   
       
}   
   
//===============================================================   
//SPI 写数据   
//IN：value   
//OUT：spi_status   
//=======================================================================   
BYTE SPI_write(BYTE value)   
{   
    unsigned char i,spi_status;   
    for(i=0;i<8;i++) {    
        if(value&0x80)    
            {   
              DDB2=0;   
              DDB2=1;   
            }   
        else    
            {   
              DDB2=0;   
              DDB2=0;   
            }      
        asm("nop"); asm("nop"); asm("nop"); asm("nop");asm("nop");   
        asm("nop"); asm("nop"); asm("nop"); asm("nop");asm("nop");   
        DDB2=1;   
        value <=1;   
        spi_status<=1;      
        if(DDB3=1)   
           spi_status=spi_status|0x01;   
        else   
            spi_status=spi_status&0xfe;   
    }   
    DDB2=0;   
    asm("nop"); asm("nop"); asm("nop"); asm("nop");asm("nop");   
    return(spi_status);   
}   
//================================================================   
//SPI读数据   
//IN：NONE   
//OUT：value   
//=======================================================================   
BYTE SPI_read()   
{   
    unsigned char i,value;   
    for(i=0;i<8;i++)    
    {    
        value <=1;   
        DDB2=1;   
        if(DDB3) value|=0x01;   
        else value&=0xFE;   
        asm("nop"); asm("nop"); asm("nop"); asm("nop");asm("nop");   
        DDB2=0;      
    }   
//  asm("nop"); asm("nop"); asm("nop"); asm("nop");asm("nop");   
    return value;   
}   
//=======================================================================   
   
//-------------------------------------------------------------------------------------------------------   
//  BYTE halSpiReadReg(BYTE addr)   
//   
//  DESCRIPTION:   
//      This function gets the value of a single specified CC1100 register.   
//   
//  ARGUMENTS:   
//      BYTE addr   
//          Address of the CC1100 register to be accessed.   
//   
//  RETURN VALUE:   
//      BYTE   
//          Value of the accessed CC1100 register.   
//-------------------------------------------------------------------------------------------------------   
BYTE halSpiReadReg(BYTE addr)    
{   
    unsigned char value;   
    DDB0=0;   
    while(DDB3);   
    DDB2=0;   
    addr|=READ_SINGLE;   
    SPI_write(addr);   
    value=SPI_read();   
    DDB0=1;   
    DDB2=0;   
    DDB2=0;   
    return value;   
}   
   
   
//-------------------------------------------------------------------------------------------------------   
//  BYTE halSpiReadStatus(BYTE addr)   
//   
//  DESCRIPTION:   
//      This function reads a CC1100 status register.   
//   
//  ARGUMENTS:   
//      BYTE addr   
//          Address of the CC1100 status register to be accessed.   
//   
//  RETURN VALUE:   
//      BYTE   
//          Value of the accessed CC1100 status register.   
//-------------------------------------------------------------------------------------------------------   
BYTE halSpiReadStatus(BYTE addr)    
{   
    unsigned char value;   
    DDB0=0;   
    while(DDB3);   
    DDB2=0;   
    addr|=READ_BURST;   
    SPI_write(addr);   
    value=SPI_read();   
    DDB0=1;   
    DDB2=0;   
    DDB2=0;   
    return value;   
}// halSpiReadStatus   
   
   
//-------------------------------------------------------------------------------------------------------   
//  void halSpiWriteReg(BYTE addr, BYTE value)   
//   
//  DESCRIPTION:   
//      Function for writing to a single CC1100 register   
//   
//  ARGUMENTS:   
//      BYTE addr   
//          Address of a specific CC1100 register to accessed.   
//      BYTE value   
//          Value to be written to the specified CC1100 register.   
//-------------------------------------------------------------------------------------------------------   
void halSpiWriteReg(BYTE addr, BYTE value)    
{   
    DDB0=0;   
    while(DDB3);   
    DDB2=0;   
    addr&=0x7F;   
    SPI_write(addr);   
    SPI_write(value);   
    DDB2=0;   
    DDB0=1;   
   
}// halSpiWriteReg   
   
   
//-------------------------------------------------------------------------------------------------------   
//  void halSpiReadBurstReg(BYTE addr, BYTE *buffer, BYTE count)   
//   
//  DESCRIPTION:   
//      This function reads multiple CC1100 register, using SPI burst access.   
//   
//  ARGUMENTS:   
//      BYTE addr   
//          Address of the first CC1100 register to be accessed.   
//      BYTE *buffer   
//          Pointer to a byte array which stores the values read from a   
//          corresponding range of CC1100 registers.   
//      BYTE count   
//          Number of bytes to be written to the subsequent CC1100 registers.   
//-------------------------------------------------------------------------------------------------------   
void halSpiReadBurstReg(BYTE addr, BYTE *buffer, BYTE count)    
{   
    unsigned char j,value;   
    DDB0=0;   
    while(DDB3);   
    DDB2=0;   
    addr|=READ_BURST;   
    SPI_write(addr);   
    for(j=0;j<count;j++)    
    {   
      value=SPI_read();   
      buffer[j]=value;   
    }   
    DDB0=1;   
}// halSpiReadBurstReg   
   
   
   
//-------------------------------------------------------------------------------------------------------   
//  void halSpiWriteBurstReg(BYTE addr, BYTE *buffer, BYTE count)   
//   
//  DESCRIPTION:   
//      This function writes to multiple CC1100 register, using SPI burst access.   
//   
//  ARGUMENTS:   
//      BYTE addr   
//          Address of the first CC1100 register to be accessed.   
//      BYTE *buffer   
//          Array of bytes to be written into a corresponding range of   
//          CCxx00 registers, starting by the address specified in _addr_.   
//      BYTE count   
//          Number of bytes to be written to the subsequent CC1100 registers.      
//-------------------------------------------------------------------------------------------------------   
void halSpiWriteBurstReg(BYTE addr, BYTE *buffer, BYTE count)   
{   
    unsigned char i,j,value;   
    DDB0=0;   
    while(DDB3);   
    DDB2=0;   
    addr|=WRITE_BURST;   
    SPI_write(addr);   
    for(j=0;j<count;j++) {   
      value=buffer[j];   
      SPI_write(value);   
      DDB2=0;    
      DDB2=0;   
      for(i=0;i<10;i++);   
    }   
    DDB0=1;   
}// halSpiWriteBurstReg   
   
   
   
   
//-------------------------------------------------------------------------------------------------------   
//  void halSpiStrobe(BYTE strobe)   
//   
//  DESCRIPTION:   
//      Function for writing a strobe command to the CC1100   
//   
//  ARGUMENTS:   
//      BYTE strobe   
//          Strobe command   
//-------------------------------------------------------------------------------------------------------   
void halSpiStrobe(BYTE strobe)    
{   
    DDB0=0;   
    while(DDB3);   
    SPI_write(strobe);   
    DDB2=0;   
    DDB2=0;   
    DDB0=1;   
   
}// halSpiStrobe   
   
   
   
//-------------------------------------------------------------------------------------------------------   
//  void halRfSendPacket(BYTE *txBuffer, UINT8 size)   
//   
   
//         
//  ARGUMENTS:   
//      BYTE *txBuffer   
//          Pointer to a buffer containg the data that are going to be transmitted   
//   
//      UINT8 size   
//          The size of the txBuffer   
//-------------------------------------------------------------------------------------------------------   
   
void halRfSendPacket(BYTE *txBuffer, UINT8 size)    
{   
    halSpiStrobe(CC1100_SFTX);   
    Dly1mS(10);   
    halSpiWriteBurstReg(CC1100_TXFIFO, txBuffer, size);   
    Dly1mS(10);   
    halSpiStrobe(CC1100_STX);   
    Dly1mS(10);   
    TMR0=0; T0IF=0;   
    while((halSpiReadStatus(CC1100_TXBYTES)&0x7F)!=0x00)       
    {   
        Dly1mS(2);   
        if(T0IF)   
        {    
            halSpiStrobe(CC1100_SIDLE);   
            return ;   
        }      
               
    }   
   
    LED1=0;   
    return ;   
}// halRfSendPacket*/   
   
   
//-------------------------------------------------------------------------------------------------------   
//  BOOL halRfReceivePacket(BYTE *rxBuffer, UINT8 *length)   
//   
    
//     
//  ARGUMENTS:   
//      BYTE *rxBuffer   
//          Pointer to the buffer where the incoming data should be stored   
//      UINT8 *length   
//          Pointer to a variable containing the size of the buffer where the incoming data should be   
//          stored. After this function returns, that variable holds the packet length.   
//             
//  RETURN VALUE:   
//      BOOL   
//          TRUE:   CRC OK   
//          FALSE:  CRC NOT OK   
//-------------------------------------------------------------------------------------------------------   
BYTE halRfReceivePacket(BYTE *rxBuffer, UINT8 length)    
{   
    BYTE status=0;   
    UINT8 packetLength;   
    status = halSpiReadStatus(CC1100_RXBYTES);   
    if((status & 0x7F)==0x00)    
    {   
           
        if(halSpiReadStatus(CC1100_MARCSTATE)!=0x0D) halSpiStrobe(CC1100_SRX);   
        return 0;   
    }   
    TMR0=0; T0IF=0;   
    while((halSpiReadStatus(CC1100_MARCSTATE)&0x1f)!=0x01)    
    {   
           
        if(T0IF)   
        {    
            halSpiStrobe(CC1100_SFRX);   
            return 0;   
        }   
    }   
    packetLength = halSpiReadReg(CC1100_RXFIFO);   
       
    if (packetLength == length) {   
        halSpiReadBurstReg(CC1100_RXFIFO, rxBuffer, packetLength);    
           
 //       *length = packetLength;   
        halSpiStrobe(CC1100_SFRX);   
           
        return 1;//(status & CRC_OK);   
    } else {   
 //       *length = packetLength;   
        halSpiStrobe(CC1100_SFRX);   
        return 0;   
    }        
}// halRfReceivePacket   
   
   
   
   
//-------------------------------------------------------------------------------------------------------   
//  void RfWriteRfSettings()   
//   
//  DESCRIPTION:   
//      This function is used to configure the CC1100 based on a given rf setting   
//   
//  ARGUMENTS:   
//         
//          Pointer to a struct containing rf register settings   
//-------------------------------------------------------------------------------------------------------   
void halRfWriteRfSettings()    
{   
    halSpiWriteReg(CC1100_FSCTRL1, 0x0C);   
    halSpiWriteReg(CC1100_FSCTRL0, 0x00);   
    halSpiWriteReg(CC1100_FREQ2, 0x10);   
    halSpiWriteReg(CC1100_FREQ1, 0xB0);   
    halSpiWriteReg(CC1100_FREQ0, 0x71);   
    halSpiWriteReg(CC1100_MDMCFG4, 0xf6);//0xc6//f5   
    halSpiWriteReg(CC1100_MDMCFG3, 0x83);   
    halSpiWriteReg(CC1100_MDMCFG2, 0x02);   
    halSpiWriteReg(CC1100_MDMCFG1, 0x22);//0x22   
    halSpiWriteReg(CC1100_MDMCFG0, 0xF8);   
    halSpiWriteReg(CC1100_CHANNR, 0x00);   
    halSpiWriteReg(CC1100_DEVIATN, 0x15);//0x30   
    halSpiWriteReg(CC1100_FREND1, 0x56);   
    halSpiWriteReg(CC1100_FREND0, 0x10);   
    halSpiWriteReg(CC1100_MCSM1, 0x00);   
    halSpiWriteReg(CC1100_MCSM0, 0x18);   
    halSpiWriteReg(CC1100_FOCCFG, 0x15);   
    halSpiWriteReg(CC1100_BSCFG, 0x6C);   
    halSpiWriteReg(CC1100_AGCCTRL2, 0x03);   
    halSpiWriteReg(CC1100_AGCCTRL0, 0x91);   
    halSpiWriteReg(CC1100_FSCAL3, 0xA9);   
    halSpiWriteReg(CC1100_FSCAL2, 0x2A);   
    halSpiWriteReg(CC1100_FSCAL0, 0x0D);   
    halSpiWriteReg(CC1100_FSTEST, 0x59);   
    halSpiWriteReg(CC1100_TEST2, 0x86);   
    halSpiWriteReg(CC1100_TEST1, 0x3D);   
    halSpiWriteReg(CC1100_TEST0, 0x09);   
    halSpiWriteReg(CC1100_IOCFG2, 0x24);   
    halSpiWriteReg(CC1100_IOCFG0, 0x06);   
    halSpiWriteReg(CC1100_PKTCTRL1, 0x04);   
    halSpiWriteReg(CC1100_PKTCTRL0, 0x05);   
    halSpiWriteReg(CC1100_ADDR, 0x00);   
    halSpiWriteReg(CC1100_PKTLEN, 0xFF);   
      
   
}   
   
//----------------------------------------------------------   
BYTE spiGetRxTxStatus(void)    
{   
    unsigned char RxTxstatus;   
    DDB0 = 0;   
    while (DDB3);   
    RxTxstatus=SPI_write(CC1100_SNOP);   
    DDB2=0;   
    DDB2=0;   
    DDB0=1;   
    return(RxTxstatus);   
}    
   
//-------------------------------------------------------------   
void POWER_UP_RESET_CC1100() {   
    unsigned char i;    
    halSpiStrobe(CC1100_SIDLE);   
    DDB0=1;   
    for(i=0;i<5;i++);     
    DDB0=0;   
    for(i=0;i<5;i++);        
    DDB0=1;   
    for(i=0;i<200;i++);     
    DDB0=0;   
    while(DDB3);   
    halSpiStrobe(CC1100_SRES);   
    DDB0=1;   
    DDB2=0;   
    DDB2=0;   
}   