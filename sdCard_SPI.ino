#include<SPI.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);   //16/8 = 2Mhz
   pinMode(MISO,INPUT);
  digitalWrite(10,LOW);

  sendByte(0xff, 10);   //dummy bytes
  
  SPI.transfer(64);   //cmd0 command
  sendByte(0x00, 4);    //arg
  SPI.transfer(0x95);            //crc byte (error response from card when crc is not sent)
  //sendByte(0xff, 2);           //response is good only when sending exact two dummy bytes
  Serial.print( getResponse() );
  Serial.println(": CMD0 Response");

  SPI.transfer(0x48);   //cmd8 command
  sendByte(0x00, 2);    //arg  //msb first
  SPI.transfer(0x01);   //arg
  SPI.transfer(0xaa);  //arg
  SPI.transfer(0x87);  //crc
  Serial.print( getResponse() );
  Serial.println(": CMD8 Response");

  //send_cmd55();
  
//  Serial.print( getResponse(), HEX );
//  Serial.print( getResponse(), HEX );
//  Serial.print( getResponse(), HEX );
//  Serial.println( getResponse(), HEX );

//  SPI.transfer(58 | 0x40);   //cmd58 command
//  sendByte(0x00, 4);    //arg
//  SPI.transfer(0xff);  //crc
//  Serial.println( getResponse() );
//  
//  Serial.println( getResponse(), HEX );
//  Serial.println( getResponse(), HEX );
//  Serial.println( getResponse(), HEX );
//  Serial.println( getResponse(), HEX );

  
//
//  res = 0xff;
//
   uint8_t res;
   
   do{
    send_cmd55();
    Serial.println("Sending ACMD41...");
    SPI.transfer(105);   //acmd41 command
    SPI.transfer(0x40);   //arg
    sendByte(0x00,3);     //arg
    SPI.transfer(0xff);  //crc
    res = getResponse();
  }while(res != 0x00);
  
  Serial.print(res);
  Serial.println(": ACMD41 response\n");

  send_cmd16();

  write_singleBlock();

  read_singleBlock();

//   do{
//    send_cmd55();
//    Serial.println("Sending ACMD41...");
//    SPI.transfer(105);   //acmd41 command
//    SPI.transfer(0x40);   //arg
//    sendByte(0x00,3);     //arg
//    SPI.transfer(0xff);  //crc
//    res = getResponse();
//  }while(res != 0x00);
//  
//  Serial.print(res);
//  Serial.println(": ACMD41 response\n");
//
//  send_cmd16();
//
//  read_singleBlock();
  
}

void loop() {
  // put your main code here, to run repeatedly:

}

void sendByte(uint8_t data, int len)
{
  for(int i = 0; i<len ;i++)
  {
    SPI.transfer(data);
  }
 }

 uint8_t getResponse()
  {
    uint8_t response = 0;
    int i = 0;
    for(int i = 0; i<30000; i++)
    {
      response = SPI.transfer(0xff);
      if(response != 0xff) return response;
    }
    return 0xff;
  }

  void send_cmd55()
  {
    Serial.println("\nSending CMD55...");
    uint8_t res;
    do
    {
      SPI.transfer(0x77);   //cmd55 command
      sendByte(0x00,4);     //arg
      SPI.transfer(0xff);  //crc
      res = getResponse();
     }while(res == 0xff);
     
     Serial.print(res);
     Serial.println(": CMD55 Response");
   }

   void send_cmd16()
   {
      sendByte(0xff, 20);    //dummy clocks
      
      SPI.transfer(80);   //cmd16 command
      sendByte(0x00, 3);    //arg
      SPI.transfer(0x01);   //arg 
      SPI.transfer(0xff);  //crc
      Serial.print( getResponse() );
      Serial.println(": CMD16 Response");
   }

   void write_singleBlock()
   {
      sendByte(0xff, 20);    //dummy clocks
      
      SPI.transfer(88);   //cmd24 command
      sendByte(0x00, 4);    //arg  //address of sector
      //SPI.transfer(0x0a);   //arg 
      //SPI.transfer(0x00);   //arg
      SPI.transfer(0xff);  //crc
      Serial.print( getResponse() );
      Serial.println(": CMD24 Response");
      
      //sendByte(0x00, 10);   //dummy clock
      SPI.transfer(0xfe);   //start token
    
      SPI.transfer(223);    //data
    //  SPI.transfer(0xff);  //crc
    //  SPI.transfer(0xff);  //crc
    
      //sendByte(0x00, 10);   //dummy clock
    
    //  SPI.transfer(77);   //cmd13 command
    //  sendByte(0x00, 4);    //arg
    //  SPI.transfer(0xff);  //crc
      Serial.print( getResponse() );
      Serial.println(": data write response\n");
   }

   void read_singleBlock()
   {
      sendByte(0xff, 20);    //dummy clocks
      
      SPI.transfer(81);   //cmd17 command
      sendByte(0x00, 4);    //arg  //address of sector
      //SPI.transfer(0x0a);   //arg 
      //SPI.transfer(0x00);   //arg
      SPI.transfer(0xff);  //crc
      Serial.print( getResponse() );
      Serial.println(": CMD17 Response");
    
       Serial.print( getResponse() );
      Serial.println(": data read response");
    
      Serial.print( getResponse() );
      Serial.println(": data read\n");
   }
