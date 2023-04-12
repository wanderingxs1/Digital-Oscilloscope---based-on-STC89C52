#include<key.h>

extern char workMode;
extern char key_num;
extern char waveMode;
extern char key_sta;
extern bit KEY1;
extern bit KEY2;

unsigned char flag4=0;//按键4修改幅值还是频率，0幅值1相位

void key_service()
  {
  if(key_sta&0x01) return;             // key_sta.0=1?????????????????￡???????????
   if(KEY2)                             //???μ???????
  	{
    key_num=key_num+1;                  //????????????0-4???????м?????????1-8??  
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
	//123号按键改变工作模式
	    case 1:
	        workMode=key_num;
	    break;
	    case 2:
	        workMode=key_num;
	    break;
	    case 3:
	        workMode=key_num;
	    break;
	//4号按键改变工作模式1下，ch2输出信号的幅值/频率（flag4=0改幅值，flag4=1改相位）,按一次按键，下次的功能也不同
	    case 4:
	        if(workMode==1){
	            if(flag4==0){
	                //修改幅值
	                flag4=1;
	                }
	            else{
	                //修改频率
	                flag4=0;
	            }
	        }
	    break;
//5678号按键适用于在工作模式1下改变输出波形
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