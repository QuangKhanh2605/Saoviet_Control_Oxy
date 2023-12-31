

#include "user_modem_init.h"
#include "user_define.h"


/*================ Func Init ==========================*/

void Init_Memory_Infor(void)
{
    //Init Timer Send data in mem
    Init_Timer_Send();
    //DCU ID
	Init_DCU_ID();
}


void Init_DCU_ID(void)
{
    uint8_t temp = 0;
	//DCU ID
	temp = *(__IO uint8_t*) ADDR_DCUID;
	if (temp != FLASH_BYTE_EMPTY)
	{
        Flash_Get_Infor ((ADDR_DCUID + 1), sModem.sDCU_id.Data_a8, &sModem.sDCU_id.Length_u16, 14);
	} else
	{
		Save_DCU_ID();
	}
}

void Save_DCU_ID(void)
{
    uint8_t i=0;
	uint8_t temp_arr[40] = {0};

	temp_arr[0] = BYTE_TEMP_FIRST;
	temp_arr[1] = sModem.sDCU_id.Length_u16;

	for (i = 0;i < sModem.sDCU_id.Length_u16; i++)
		temp_arr[i+2] = sModem.sDCU_id.Data_a8[i];

    OnchipFlashPageErase(ADDR_DCUID);
    OnchipFlashWriteData(ADDR_DCUID, temp_arr, 40);
}



void Init_Timer_Send (void)
{
    uint8_t     temp = 0;
    uint8_t 	Buff_temp[24] = {0};
    uint8_t 	Length = 0;

    temp = *(__IO uint8_t*) (ADDR_FREQ_ACTIVE);
    //Check Byte EMPTY
    if (temp != FLASH_BYTE_EMPTY)
    {
        OnchipFlashReadData(ADDR_FREQ_ACTIVE, &Buff_temp[0], 24);
        Length = Buff_temp[1];
        if (Length < 24)
        {
            //Get Freq Save Power
            sFreqInfor.NumWakeup_u8 = Buff_temp[2];
            
            sFreqInfor.FreqWakeup_u32 = Buff_temp[3];
            sFreqInfor.FreqWakeup_u32 = (sFreqInfor.FreqWakeup_u32 << 8) | Buff_temp[4];
            
            //Get Freq Online
            sFreqInfor.FreqSendOnline_u32 = Buff_temp[5];
            sFreqInfor.FreqSendOnline_u32 = sFreqInfor.FreqSendOnline_u32 << 8 | Buff_temp[6];
            sFreqInfor.FreqSendOnline_u32 = sFreqInfor.FreqSendOnline_u32 << 8 | Buff_temp[7];
            sFreqInfor.FreqSendOnline_u32 = sFreqInfor.FreqSendOnline_u32 << 8 | Buff_temp[8];
        }    
    } else
    {
        Save_Freq_Send_Data();
    }
}

 


void Save_Freq_Send_Data (void)
{
    uint8_t Buff_temp[24] = {0};

    Buff_temp[0] = BYTE_TEMP_FIRST;
    Buff_temp[1] = 7;
    Buff_temp[2] = sFreqInfor.NumWakeup_u8;

    Buff_temp[3] = (sFreqInfor.FreqWakeup_u32 >> 8) & 0xFF;
    Buff_temp[4] = sFreqInfor.FreqWakeup_u32 & 0xFF;
    
    Buff_temp[5] = (sFreqInfor.FreqSendOnline_u32 >> 24) & 0xFF;
    Buff_temp[6] = (sFreqInfor.FreqSendOnline_u32 >> 16) & 0xFF;
    Buff_temp[7] = (sFreqInfor.FreqSendOnline_u32 >> 8) & 0xFF;
    Buff_temp[8] = sFreqInfor.FreqSendOnline_u32 & 0xFF;

    Erase_Firmware(ADDR_FREQ_ACTIVE, 1);
    OnchipFlashWriteData(ADDR_FREQ_ACTIVE, &Buff_temp[0], 16);
}



uint16_t mInit_u16 (uint32_t Addr, uint16_t MAX)
{
    uint8_t temp = 0;
    uint8_t Buff_temp[20] = {0};
    uint16_t IndexFind = 0;
    
    //Doc vi tri send va luu ra
    temp = *(__IO uint8_t*) Addr;
    
	if (temp != FLASH_BYTE_EMPTY)
	{
        OnchipFlashReadData ((Addr + 2), Buff_temp, 2);   
        IndexFind = (Buff_temp[0] << 8) | Buff_temp[1];
      
        //kiem tra dieu kien gioi han InDex
        if (IndexFind >= MAX)
        	IndexFind = 0;
        
    } else
    {
    	mSave_u16(Addr, 0);
    }
    
    return IndexFind;
}


void mSave_u16 (uint32_t Addr, uint16_t Val)
{
    uint8_t aTemp[4] = {0};

    aTemp[0] = (uint8_t) (Val >> 8);
    aTemp[1] = (uint8_t) Val;

    Save_Array(Addr, &aTemp[0], 2);
}





