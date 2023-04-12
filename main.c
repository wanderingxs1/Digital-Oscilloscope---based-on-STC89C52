#include<main.h>
#include<adc.h>

sbit KEY1      = P3^4;
sbit KEY2      = P3^5;
//---------------------------------------------------------------------------------------
void init_timer0(void)              //  ��ʱ��0��ʼ
  {
  TMOD &= 0XF0;
  TMOD |= 0X02;	   //8λ���ض�ʱ��t0
  TL0 = 0X06;
  TH0 = 0X06;	   //һ����������0.25ms
  TR0 = 1;
  }
//---------------------------------------------------------------------------------------
void init_special_interrupts(void)   //  �ж�����
  {
  //�����ж�����
  EA  = 1;		//���ж�
  ET0 = 1;		//����T0�ж�
  EADC=1;		//����AD�ж�
  //�ж����ȼ�����
  PT0 = 1;
  PADC=0;
  }
//---------------------------------------------------------------------------------------
void dsptask()
  {
  unsigned char i;
  unsigned char a,b;

  switch(sel)
   {
   case 0:  a=0x01;  break;
   case 1:  a=0x02;  break;
   case 2:  a=0x04;  break;
   default: a=0x08;
   }
 for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7f;
    }
  a=dspbuf[sel];
  key_num=sel;		//ȡ�ð�������������
  sel++;
  if(sel>=4) sel=0;
  for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7fff;
    }
  D_RCLK=0;
  D_RCLK=1;
  D_RCLK=0;

  //����Ƭѡ�ź�
  a=0x10;
  for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7f;
    }
   //ѡ��Ҫ��ʾ�Ĺ���ģʽ
  switch(workMode)
   {
   case 0:  a=0xF7;  break;
   case 1:  a=0xFB;  break;
   case 2:  a=0xFD;  break;
   default: a=0x0E;
   }
   //�ѹ���ģʽ�ͳ�ȥ
  for(b=0x80,i=0;i<8;i++)
    {
    if(a&b)  D_SER=1;
    else     D_SER=0;
    D_SRCLK=0;
    D_SRCLK=1;
    D_SRCLK=0;
    b=b>>1;  b=b&0x7fff;
    }
  D_RCLK=0;
  D_RCLK=1;
  D_RCLK=0;
  }
//---------------------------------------------------------------------------------------
void timer_isr() interrupt 1           //��ʱ��0�жϴ���
  {
  EA=0;
  adcount++;
  adc_start();
  if(adcount==20)                      // 1/4*20 = 5ms
    {
    dsptask();
    key_service();
    clocktime++;
    adcount=0;
    }
  EA=1;
  }
//---------------------------------------------------------------------------------------
void fdisp(unsigned char n,unsigned char m)      //�����ǽ�Ҫд�����ֵn��Ӧ������ܱ���װ��dspbuf�ĵ�m����Ԫ,û���õ�
  {
  char  c;
   switch(n)
    {
    case 0:  c=0x11;  break;
    case 1:  c=0x7d;  break;
    case 2:  c=0x23;  break;
    case 3:  c=0x29;  break;
    case 4:  c=0x4d;  break;
    case 5:  c=0x89;  break;
    case 6:  c=0x81;  break;
    case 7:  c=0x3d;  break;
    case 8:  c=0x01;  break;
    default: c=0x09; 
    }
   dspbuf[m]=c;
  }
//---------------------------------------------------------------------------------------
void main(void)                    // ������
  {
  init_timer0();                  //��ʼ����ʱ��0
  init_special_interrupts();      //�����ж�
  adc_init();						//����ADC
  waveInit();
  for(;;)
    {

  //����ģʽ1�Ż���ʱ���д洢
  if(workMode==1){
	//���ADC����RESULT��ֵ����һ�β�ͬ���Ǿ͸���,������һ��ѭ������
	if(ad_temp!=ADC_RESULT){
  daAddress=adAddress;//ͬ��
	if(adAddress<=0x0FFF){
	XBYTE[adAddress]=ADC_RESULT;
	ad_temp=ADC_RESULT;
	adAddress++;
	}
	else{
	adAddress=ADC_BASE_ADDRESS;
	XBYTE[adAddress]=ADC_RESULT;
	ad_temp=ADC_RESULT;
    adAddress++;
	}
	}
  }

	//����ģʽ1�Ĳ���ѡ��
	//����ģʽ2��������в���
  //���ǽ����������������Լ�ά����buffer
	switch(workMode){
	case 1:
	{
		//ͨ��1ʵʱ���
		DAC_VALUE=ADC_RESULT;
		//ͨ��2������ֲ�����WAVE_VALUE��ֵ
		switch(waveMode){
			case 1:
      {
        if(sinAddress<=0x10FF){
      			WAVE_VALUE=XBYTE[sinAddress];
      			sinAddress++;
    		  }
    	  else{
      		sinAddress=SIN_BASE_ADDRESS;
      		WAVE_VALUE=XBYTE[sinAddress];
      		sinAddress++;
    	}
      }
      break;
			case 2:
       {
        if(triAddress<=0x11FF){
      			WAVE_VALUE=XBYTE[triAddress];
      			triAddress++;
    		  }
    	  else{
      		triAddress=TRI_BASE_ADDRESS;
      		WAVE_VALUE=XBYTE[triAddress];
      		triAddress++;
    	}
      }
      break;
			case 3:
      {
        if(squAddress<=0x12FF){
      			WAVE_VALUE=XBYTE[squAddress];
      			squAddress++;
    		  }
    	  else{
      		squAddress=SQU_BASE_ADDRESS;
      		WAVE_VALUE=XBYTE[squAddress];
      		squAddress++;
    	}
      }
      break;
			case 4:
      {
        if(teeAddress<=0x13FF){
      			WAVE_VALUE=XBYTE[teeAddress];
      			teeAddress++;
    		  }
    	  else{
      		teeAddress=TEE_BASE_ADDRESS;
      		WAVE_VALUE=XBYTE[teeAddress];
      		teeAddress++;
    	}
      }
      break;
			default:break;
		}
	}
	break;
	case 2:
	{
		//ͨ��1ʵʱ���
		DAC_VALUE=ADC_RESULT;
		//ͨ��2�����ǰ�ɵĲ���
		//����ģʽ2����ch2�������OUTPUT_VALUE�ĸ�ֵ,��������ģʽ2һ���Ѿ�����һ����
  		//����һ�����⣬��������ģʽ1��adAddress�����ڴ�ռ��ĳһ��ͣ��
  		//�ݶ�Ϊ��adAddress��ʼѭ�����
  		if(workMode==2){
    		if(daAddress<=0x0FFF){
      			OUTPUT_VALUE=XBYTE[daAddress];
      			daAddress++;
    		}
    	else{
      		daAddress=ADC_BASE_ADDRESS;
      		OUTPUT_VALUE=XBYTE[daAddress];
      		daAddress++;
    	}
  		}
	}
	break;
  case 3:
  {
    //ͨ��1ʵʱ���
    DAC_VALUE=ADC_RESULT;
  }
  break;
	default:break;
	}

  //��������
  if(key_sta&0x01)              // key_sta.0==1?���а�������
    {             
		    keyWork();
        key_sta=key_sta&0xfe;           // ��key_sta.0=0,��λ
    } 
	}
  }
//----------------------------------End---------------------------------------------------
