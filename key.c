#include<key.h>

extern char workMode;
extern char key_num;
extern char waveMode;
extern char key_sta;
extern bit KEY1;
extern bit KEY2;

unsigned char flag4=0;//����4�޸ķ�ֵ����Ƶ�ʣ�0��ֵ1��λ

void key_service()
  {
  if(key_sta&0x01) return;             // key_sta.0=1?????????????????��???????????
   if(KEY2)                             //???��???????
  	{
    key_num=key_num+1;                  //????????????0-4???????��?????????1-8??  
	key_sta=key_sta|0x01;              // ??key_sta.0=1
	}
   else if(KEY1)
    	  {
	      key_num=key_num+5;   
          key_sta=key_sta|0x01;        // ??key_sta.0=1
	      }
  }

void keyWork()
{
	switch(key_num)
	{
	//123�Ű����ı乤��ģʽ
	    case 1:
	        workMode=key_num;
	    break;
	    case 2:
	        workMode=key_num;
	    break;
	    case 3:
	        workMode=key_num;
	    break;
	//4�Ű����ı乤��ģʽ1�£�ch2����źŵķ�ֵ/Ƶ�ʣ�flag4=0�ķ�ֵ��flag4=1����λ��,��һ�ΰ������´εĹ���Ҳ��ͬ
	    case 4:
	        if(workMode==1){
	            if(flag4==0){
	                //�޸ķ�ֵ
	                flag4=1;
	                }
	            else{
	                //�޸�Ƶ��
	                flag4=0;
	            }
	        }
	    break;
//5678�Ű����������ڹ���ģʽ1�¸ı��������
	    case 5:
	        if(workMode==1){
                waveMode=1;
            }
        break;
        case 6:
            if(workMode==1){
                waveMode=2;
            }
        break;
        case 7:
            if(workMode==1){
                waveMode=3;
            }
        break;
        case 8:
            if(workMode==1){
                waveMode=4;
            }
        break;
        default:
        break;

	}
}