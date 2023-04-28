#include<key.h>
//按键延时消除抖动，很重要！避免一次操作被误判为多次
extern char workMode;
extern char key_num;
extern char waveMode;
extern char key_sta;
extern char isChange;//是准备调频/调幅，改变按钮5678的用途
extern char freBuffer;
extern char ampBuffer;
sbit KEY1      = P3^4;
sbit KEY2      = P3^5;


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
	//4号按键改变工作模式1下,是否进行频率和幅度的调整，相关变量isChange
	    case 4:
	        if(workMode==1){
	            if(isChange==0){
					isChange=1;//开始调频调幅
	                }
	            else{
					isChange=0;
					freBuffer=0;
					ampBuffer=1;
	            }
	        }
			delay(20);
	    break;
//5678号按键适用于在工作模式1下改变输出波形
	    case 5:
	        if(workMode==1){
				if(!isChange){
					waveMode=1;
				}
				else{
					if(freBuffer>=1){
						freBuffer=freBuffer-1;
					}
				}
            }
			delay(20);
        break;
        case 6:
            if(workMode==1){
				if(!isChange){
					waveMode=2;
				}
				else{
					freBuffer=freBuffer+1;
				}
            }
			delay(20);
        break;
        case 7:
            if(workMode==1){
				if(!isChange){
					waveMode=3;
				}
				else{
					if(ampBuffer>=2){
						ampBuffer=ampBuffer-1;
					}
				}
            }
			delay(30);
        break;
        case 8:
            if(workMode==1){
				if(!isChange){
					waveMode=4;
				}
				else{
					ampBuffer=ampBuffer+1;
				}
            }
			delay(30);
        break;
        default:
        break;

	}
}