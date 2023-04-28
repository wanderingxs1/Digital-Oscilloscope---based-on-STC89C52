#include<featureExtract.h>

extern char ADC_RESULT;
extern int adAddress;

unsigned int freq=0;//�����ź�Ƶ��,0-300Hz
float vpp=0.0;//���Ե�ѹֵ
unsigned char amp=0;//��һʱ�̵Ĳ�������ֵ
unsigned char ampl=0;//ǰһ�εĲ�������ֵ
//������ֵһ��Ҫ��ʼ��Ϊ��ֵ��
unsigned char amp_up=128;//���ݿ��ڵ���ߵ�ѹֵ
unsigned char amp_low=128;//���ݿ��ڵ���͵�ѹֵ
int fre=0;//���ݿ��������������ڵĲ������ܸ���
int fre_up=0;//һ�����ڵ�ĩ��ַ
int fre_low=0;//һ�����ڵ��׵�ַ
float fre_count=0;//�Ѿ����������ڸ���

void ampMeasure()
{
    amp=ADC_RESULT;
    if (amp>amp_up){amp_up=amp;}
    if (amp<amp_low){amp_low=amp;}
	//������ΧΪһ�����ݿ�
    if(adAddress>0x0400){
        vpp=(amp_up*5.0-amp_low*5.0)/128;//֮ǰΪ��ʾ������ʾ����ֵ��2������˻���
        amp_up=amp_low=128;
    }
}

void freMeasure()
{
    amp=ADC_RESULT;
	//��ͼ�ж�ʱ��
	//�ж�һ�����ڵĿ�ʼ����ǰһ��С��0����һ�δ���0
    if (amp>128 && ampl<=128)
    {
    fre_up=adAddress; //��fre_count�����ڵ��׵�ַ
    if(fre_low!=0)	  //��0������֮��Ż����
    {
    fre=fre+fabs(fre_low-fre_up);//��һ��fabs()��һ�����ڵĲ��������,fre�����ݿ��������������ڵĲ������ܸ���
    fre_count++;
    }
    fre_low=fre_up;	//����Ϊ��(fre_count+1)	�����ڵ��׵�ַ
    }
	//���ݿ鴦����
    if(adAddress>0x0400){
		//������طŲ��δ洢�Ĳ���Ƶ��Ϊ4000/2=2000Hz��ע�����Ƶ��
        freq=floor(2000/(fre*1.0/fre_count));
        fre=0;
        fre_up=fre_low=0;
        fre_count=0;
        amp=ampl=129;
    }
    ampl=amp;

}