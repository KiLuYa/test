#ifndef MCU_NetworkInterface_H
#define MCU_NetworkInterface_H


//#define MCU_Network_SERVER_IP "127.0.0.1"  //ʹ�ñ���Ϊ "127.0.0.1";ʾ�̺�:"192.168.1.206"

//#define MCU_Network_SERVER_PORT 6000


//#define		MCU_Network_Error_Disconnect					-300	//�������ӶϿ�
//#define		MCU_Network_Error_DataMismatch				-299	//�������ݰ���ƥ��
//#define		MCU_Network_Error_DownloadNoPath			-298 //���������Ҳ����ļ�
//#define		MCU_Network_Error_DownloadOpenFile		-297 //�����������ļ�����

#if defined(__cplusplus)
extern "C"
{
#endif


//����˵����
//1�����䱨����Ϣ��MC״̬��Ϣ����ʼ������Ϣ�仯ʱ���䣩
//2�����ݰ�����Ϣ�ش����ļ����书��
//3������ͨѶ�쳣����Ҫ���Զ����ӹ��ܣ���֤���������жϣ�,��Ϣ�����쳣��Ҫ����


//ͨѶ��ʽ���ڴ��б仯ʱ���ͣ�����Ҫ���ڷ��ͣ����ڼ������״̬��
//������������� MCU �� MC ����������ռ�ÿռ��С������ṹ��
typedef struct MCU_NetworkOutputDataType   //���MC
{
	char modeOfMC;	//0 program, 1 autoRun, 2 Teach , 3 Remote
	char emg;				//0 ��ʾ�޼�ͣ��1��ʾ��ͣ״̬
	char feedbackBackUp;//0��̬�򱸷ݽ�����1׼�����ݣ�2������
	int errorNo;
	
	

//	int click;	//MC����һ�����ݺ��������ʾ+1 Ҳ�����ڼ��ͨѶ�쳣
}MCU_NetworkOutputDataType;

typedef struct MCU_NetworkInputDataType
{	
	char triggerBackup;//0��̬�򱸷ݽ�����1�������ݣ�2���Կ�ʼ���ݣ�
	
	int click;	//MCU����һ�����ݺ�������ʶ+1 Ҳ�����ڼ��ͨѶ�쳣
}MCU_NetworkInputDataType;

typedef struct MCU_NetworkOperateDataType
{	
	int tmp;
	
	
	char pBuf_Revc[64];
	int flagPos_Recv;
	int size_Recv;
	
}MCU_NetworkOperateDataType;



#ifdef outputData_MCUNetwork_GLOBALS
#define outputData_MCUNetwork_EXT
#else
#define outputData_MCUNetwork_EXT extern
#endif
outputData_MCUNetwork_EXT MCU_NetworkOutputDataType outputData_MCUNetwork;


#ifdef inputData_MCUNetwork_GLOBALS
#define inputData_MCUNetwork_EXT
#else
#define inputData_MCUNetwork_EXT extern
#endif
inputData_MCUNetwork_EXT MCU_NetworkInputDataType inputData_MCUNetwork;

#ifdef operateData_MCUNetwork_GLOBALS
#define operateData_MCUNetwork_EXT
#else
#define operateData_MCUNetwork_EXT extern
#endif
operateData_MCUNetwork_EXT MCU_NetworkOperateDataType operateData_MCUNetwork;

//���ظ�������ʾ�쳣��>=0Ϊ��ʼ���ɹ�
int MCU_NetworkInit(MCU_NetworkOperateDataType *pOpData,MCU_NetworkInputDataType *pInput,MCU_NetworkOutputDataType *pOutput);


void MCU_NetworkExecuteCycle(MCU_NetworkOperateDataType *pOpData,MCU_NetworkInputDataType *pInput,MCU_NetworkOutputDataType *pOutput);


void MCU_NetworkClose(MCU_NetworkOperateDataType *pOpData,MCU_NetworkInputDataType *pInput,MCU_NetworkOutputDataType *pOutput);


int MCU_ProcessRevcString(char *pInputBuf, int lenBuf);
//���ط����ֽ���
int MCU_GetSendString(char *pInputBuf);


int MCU_atoi(char *str);


//@param n number to be convered into str
//@param str pointer to a buffer provided by caller
//@param numChar �����ַ����ĳ��ȣ��������ĸ���
void MCU_itoa(int n, char *str, int numChar);
#if defined(__cplusplus)
}
#endif 

#endif
