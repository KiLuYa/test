#include "tm1637.h"
#include "global.h"


//SDA��������
#define TM_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
#define TM_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO����
#define TM_IIC_SCL    PBout(6) //SCL
#define TM_IIC_SDA    PBout(7) //SDA	 
#define TM_READ_SDA   PBin(7)  //����SDA


u8 CODE[19] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,\
	0x6f,0x77,\
	0x7C,0x39,0x5E,0x79,0x71,0x76,0x38,0x73};	 //0--18

	
//��ʼ��IIC
static void TM_IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
}
//����IIC��ʼ�ź�
static void TM_IIC_Start(void)
{
	TM_SDA_OUT();		//SDA�����ģʽ
	TM_IIC_SDA=1;
	TM_IIC_SCL=1;
	delay_us(4);
 	TM_IIC_SDA=0;		//start singal: when SCL is high, SDA change form high to low 
	delay_us(4);
	TM_IIC_SCL=0;		//ǯסI2C���ߣ�׼�����ͻ��������
	delay_us(4);
}
//����IICֹͣ�ź�
static void TM_IIC_Stop(void)
{
	TM_SDA_OUT();		//SDA�����ģʽ
	TM_IIC_SCL=0;
	delay_us(2);
	TM_IIC_SDA=0;
 	delay_us(2);
	TM_IIC_SCL=1;
	delay_us(4);
	TM_IIC_SDA=1;		//stop singal: when SCL is high, SDA change form low to high
	delay_us(4);
}

/*
//IIC����һ���ֽ�
//��Ҫ����ӻ����ص�ACK
//����ֵ��0��ʾ�ɹ��յ��ӻ�Ӧ�𣬷�0ֵ��ʾû���յ��ӻ�Ӧ��ͨ�ų���
static u8 TM_IIC_Send_Byte(u8 txd)
{
    u8 t;
	u8 ErrTime=0;
	
	TM_SDA_OUT();
    for(t=0;t<8;t++)
    {
		TM_IIC_SCL=0;		//����ʱ�ӿ�ʼ���ݴ���
		delay_us(2);
        TM_IIC_SDA = txd&0x01;
        txd>>=1;
		delay_us(4);
		TM_IIC_SCL=1;
		delay_us(2);
    }

	TM_SDA_IN();		//SDA����Ϊ����  
	TM_IIC_SDA=1;delay_us(2);	   
	TM_IIC_SCL=0;delay_us(2);	//�ڵ�8��ʱ���½��أ��ӻ�����Ӧ��
	while(TM_READ_SDA)
	{
		ErrTime++;
		if(ErrTime>250)
			return 1;
	}
	TM_IIC_SCL=1;
	delay_us(2);
	return 0;
} */

static u8 TM_IIC_Send_Byte(u8 txd)
{
	u8 t;
	u8 ErrTime=0;
	
	TM_SDA_OUT(); 	    
    TM_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        TM_IIC_SDA=txd&0x01;
        txd>>=1;
		delay_us(4);
		TM_IIC_SCL=1;
		delay_us(2); 
		TM_IIC_SCL=0;	
		delay_us(2);
    }

	TM_SDA_IN();		//SDA����Ϊ����  
	TM_IIC_SDA=1;delay_us(2);	   
	TM_IIC_SCL=0;delay_us(2);	//�ڵ�8��ʱ���½��أ��ӻ�����Ӧ��
	while(TM_READ_SDA)
	{
		ErrTime++;
		if(ErrTime>250)
			return 1;
	}
	TM_IIC_SCL=1;
	delay_us(2);
	return 0;
}
//IIC��ȡһ���ֽ�
//��Ҫ�ڶ�ȡ��һ���ֽں󣬸��ӻ�����һ��ACK
static u8 TM_IIC_Read_Byte(void)
{
	u8 i, recv=0;
	
	TM_SDA_IN();	//SDA����Ϊ����ģʽ
    for(i=0;i<8;i++)
	{
        TM_IIC_SCL=0; 
        delay_us(2);
		TM_IIC_SCL=1;
		delay_us(2);
        recv>>=1;
        if(TM_READ_SDA) recv|=0x80;
    }
	TM_IIC_SCL=0;
	TM_SDA_OUT();
	TM_IIC_SDA=0;
	delay_us(2);
	TM_IIC_SCL=1;
	delay_us(2);
	TM_IIC_SCL=0;
	delay_us(2);

    return recv;
}

void TM1637_Init(void)
{
	TM_IIC_Init();
	TM_IIC_Start();
	TM_IIC_Send_Byte(0x8C);	//��ʾ�����������ʾ��������Ϊ11/16.
	TM_IIC_Stop();
}

void TM1637_SetDigit(u8 digit, u8 c)
{
	TM_IIC_Start();
	TM_IIC_Send_Byte(0x44);		//�����������ã��̶���ַ��д���ݵ���ʾ�Ĵ���
	TM_IIC_Stop();

	TM_IIC_Start();
	TM_IIC_Send_Byte(digit);	//��ַ�������ã�д��addr��Ӧ��ַ
	TM_IIC_Send_Byte(c);		//��addr��ַд����
	TM_IIC_Stop();
}

u8 TM1637_Scan_Key(void)
{
	u8 key;
	
	TM_IIC_Start();
	TM_IIC_Send_Byte(0x42);	//д����ָ��0x42
	key = TM_IIC_Read_Byte();
	TM_IIC_Stop();

	return key;
}
