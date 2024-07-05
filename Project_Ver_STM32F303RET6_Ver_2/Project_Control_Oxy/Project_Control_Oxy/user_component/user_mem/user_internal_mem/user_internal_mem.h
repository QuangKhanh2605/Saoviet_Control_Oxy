/*
    8/2021
    Thu vien xu ly Uart
*/
#ifndef USER_INTERNAL_ONFLASH_H
#define USER_INTERNAL_ONFLASH_H

#include "user_util.h"
#include "onchipflash.h"

#include "event_driven.h"
#include "queue_p.h"

#include "user_mem_comm.h"

//===============Define address=================
#if defined (STM32L433xx)
    #define ON_FLASH_BASE               0x08000000
    #define	ADDR_FLASH_END			    0x0803F800
    /*========Add Firmware: Main + Update=========== */
    #define	ADDR_MAIN_PROGRAM			0x08004000	        //43 Page cho chuong trinh chinh
    #define	ADDR_UPDATE_PROGRAM			0x08019800	        //43 page -> End Program = 0x0802F000

    #define ADDR_FLAG_HAVE_NEW_FW       0x08030000

    /*========Add Record Mess ================ */
    #define	ADDR_MESS_A_START   		0x08031000	         //4 page * 2K = 8 K
    #define	ADDR_MESS_A_STOP			0x08033000

    #define	ADDR_EVENT_START   		    0x08033000	         //2 page * 2K = 4 K
    #define	ADDR_EVENT_STOP			    0x08034000

    #define	ADDR_LOG_START   		    0x08034000	         //6 page * 2K = 12 K
    #define	ADDR_LOG_STOP			    0x08037000           //64*200 = 12800 = hơn 12k .

    /*=======Add Index Mess================ */
    #define	ADDR_INDEX_LOG_SEND 		0x08037000
    #define	ADDR_INDEX_LOG_SAVE 	    0x08037800

    #define	ADDR_INDEX_EVENT_SEND 		0x08038000
    #define	ADDR_INDEX_EVENT_SAVE 	    0x08038800

    #define	ADDR_INDEX_TSVH_SEND 		0x08039000
    #define	ADDR_INDEX_TSVH_SAVE 		0x08039800

    #define	ADDR_INDEX_GPS_SEND 		0x0803A000      
    #define	ADDR_INDEX_GPS_SAVE 		0x0803A800

    /* ============= Addr Meter config======== */
    #define	ADDR_FLAG_RESET		        0x0803B000
    #define	ADDR_METER_NUMBER			0x0803B800

    #define	ADDR_LEVEL_CALIB			0x0803C000	        
    #define	ADDR_THRESH_MEAS			0x0803C800	

    /* ============= Addr Modem config ======== */
    #define	ADDR_SERVER_INFOR			0x0803E000
    #define	ADDR_SERVER_BACKUP			0x0803E800
    #define	ADDR_FREQ_ACTIVE    	    0x0803F000
    #define ADDR_DCUID         			0x0803F800        // sDCU.sDCU_id

    #define	FLASH_MAX_MESS_TSVH_SAVE	60
    #define	FLASH_MAX_MESS_EVENT_SAVE   60
    #define FLASH_MAX_MESS_LOG_SAVE     150

    #define MAX_SIZE_FIRMWARE           84     //Kb
    #define FIRMWARE_SIZE_PAGE	        MAX_SIZE_FIRMWARE*1024/FLASH_PAGE_SIZE

    #define SIZE_DATA_TSVH	            128        
    #define SIZE_DATA_EVENT	            64         
    #define SIZE_DATA_LOG	            64         
#endif


/*=========== define cho chip L151 ==================*/

#if defined (STM32L151xC)
    #define ON_FLASH_BASE               0x08000000
    #define ADDR_FLASH_END              0x0803FF00 
    /*========Add Firmware: Main + Update=========== */
//    #define	ADDR_MAIN_PROGRAM			0x08002000	        //47 Page cho chuong trinh chinh
//    #define	ADDR_UPDATE_PROGRAM			0x08019800	        //47 page -> End Program = 0x08031000
//
//    #define ADDR_FLAG_HAVE_NEW_FW       0x08031800

    #define	ADDR_MAIN_PROGRAM			0x08002C00	        //47 Page cho chuong trinh chinh
    #define	ADDR_UPDATE_PROGRAM			0x0801D800	        //47 page -> End Program = 0x08031000

    #define ADDR_FLAG_HAVE_NEW_FW       0x08038600

    /*========Add Record Mess ================ */
    #define	ADDR_MESS_A_START   		0x08038700	         //8 K
    #define	ADDR_MESS_A_STOP			0x0803A700

    #define	ADDR_EVENT_START   		    0x0803A800	         //8 K
    #define	ADDR_EVENT_STOP			    0x0803C800

    #define	ADDR_LOG_START   		    0x0803C900	         //8 K
    #define	ADDR_LOG_STOP			    0x0803E900           //64*128 = 8192 = 8k .

    /*=======Add Index Mess================ */
    #define	ADDR_INDEX_LOG_SEND 		0x0803EA00      
    #define	ADDR_INDEX_LOG_SAVE 	    0x0803EB00

    #define	ADDR_INDEX_EVENT_SEND 		0x0803EC00      
    #define	ADDR_INDEX_EVENT_SAVE 	    0x0803ED00

    #define	ADDR_INDEX_TSVH_SEND 		0x0803EE00      
    #define	ADDR_INDEX_TSVH_SAVE 		0x0803EF00  
    
//    #define	ADDR_INDEX_GPS_SEND 		0x0803D000      
//    #define	ADDR_INDEX_GPS_SAVE 		0x0803D100    

    /* ============= Addr Meter config======== */
    //Setting Wm
//    #define	ADDR_METER_NUMBER			0x0803DB00
//
//    #define	ADDR_LEVEL_CALIB			0x0803DC00	        
//    #define	ADDR_THRESH_MEAS			0x0803DE00
//    #define ADDR_SLAVE_ID               0x0803DF00



    //Setting Emeter
//    #define ADDR_PF_SETTING             0x0803F200        
//    #define ADDR_VOL_SETTING         	0x0803F300        
//    #define ADDR_CT_SETTING             0x0803F400        
//    #define ADDR_TIME_SETTING           0x0803F500        
//    #define ADDR_RELAY_SETTING          0x0803F600    

    //Register Inverter 
    #define ADDR_SAVE_SALINITY          0x0803F000
    #define ADDR_OXY_GETTICK            0x0803F100
    #define ADDR_STATE_WARNING          0x0803F200
    #define ADDR_FLAG_RESET_DCU         0x0803F300
    #define ADDR_CTRL_OXY_RUN_FREE      0x0803F400
    #define ADDR_PASSWORD_SETTING       0x0803F500
    #define ADDR_TIME_CONTROL_SETTING   0x0803F600
    #define ADDR_OXY_THRESH_SETTING     0x0803F700
    #define ADDR_INFOR_SLAVE_RS485      0x0803F800
    #define ADDR_INIT_STATE_MACHINE     0X0803F900

    /* ============= Add Modem config ======== */
    #define	ADDR_SERVER_INFOR			0x0803FA00
    #define	ADDR_SERVER_BACKUP			0x0803FB00
    #define	ADDR_FREQ_ACTIVE    	    0x0803FC00
    #define ADDR_DCUID         			0x0803FD00       
    
    /*======= Define Value===========*/   
    #define	FLASH_MAX_MESS_TSVH_SAVE	30
    #define	FLASH_MAX_MESS_EVENT_SAVE   30
    #define FLASH_MAX_MESS_LOG_SAVE     128

    #define MAX_SIZE_FIRMWARE           106     //Kb
    #define FIRMWARE_SIZE_PAGE	        MAX_SIZE_FIRMWARE*1024/FLASH_PAGE_SIZE

    #define SIZE_DATA_TSVH	            128        
    #define SIZE_DATA_EVENT	            64         
    #define SIZE_DATA_LOG	            64   

#endif

/*=========== define cho chip F303 ==================*/

#if defined (STM32F303xE)
    #define ON_FLASH_BASE               0x08000000
    #define ADDR_FLASH_END              0x0807F800
    /*========Add Firmware: Main + Update=========== */
//    #define	ADDR_MAIN_PROGRAM			0x08002000	        //47 Page cho chuong trinh chinh
//    #define	ADDR_UPDATE_PROGRAM			0x08019800	        //47 page -> End Program = 0x08031000
//
//    #define ADDR_FLAG_HAVE_NEW_FW       0x08031800

    #define	ADDR_MAIN_PROGRAM			0x08004000	        //75 Page cho chuong trinh chinh (150 Kb)
    #define	ADDR_UPDATE_PROGRAM			0x08029800	        //75 page -> End Program = 0x0804F000

    #define ADDR_FLAG_HAVE_NEW_FW       0x0804F000

    /*========Add Record Mess ================ */
    #define	ADDR_MESS_A_START   		0x08064000	         //8 K
    #define	ADDR_MESS_A_STOP			0x08066000

    #define	ADDR_EVENT_START   		    0x08066800	         //8 K
    #define	ADDR_EVENT_STOP			    0x08068800

    #define	ADDR_LOG_START   		    0x08069000	         //8 K
    #define	ADDR_LOG_STOP			    0x0806B000           //64*128 = 8192 = 8k .

    /*=======Add Index Mess================ */
    #define	ADDR_INDEX_LOG_SEND 		0x0806B800      
    #define	ADDR_INDEX_LOG_SAVE 	    0x0806C000

    #define	ADDR_INDEX_EVENT_SEND 		0x0806C800      
    #define	ADDR_INDEX_EVENT_SAVE 	    0x0806D000

    #define	ADDR_INDEX_TSVH_SEND 		0x0806D800      
    #define	ADDR_INDEX_TSVH_SAVE 		0x0806E000  
    
//    #define	ADDR_INDEX_GPS_SEND 		0x0803D000      
//    #define	ADDR_INDEX_GPS_SAVE 		0x0803D100    

    /* ============= Addr Meter config======== */
    //Setting Wm
//    #define	ADDR_METER_NUMBER			0x0803DB00
//
//    #define	ADDR_LEVEL_CALIB			0x0803DC00	        
//    #define	ADDR_THRESH_MEAS			0x0803DE00
//    #define ADDR_SLAVE_ID               0x0803DF00



    //Setting Emeter
//    #define ADDR_PF_SETTING             0x0803F200        
//    #define ADDR_VOL_SETTING         	0x0803F300        
//    #define ADDR_CT_SETTING             0x0803F400        
//    #define ADDR_TIME_SETTING           0x0803F500        
//    #define ADDR_RELAY_SETTING          0x0803F600    

    /* ============= Regiter Machine ============*/
    #define ADDR_SAVE_SALINITY          0x0806E800
    #define ADDR_OXY_GETTICK            0x0806F000
    #define ADDR_STATE_WARNING          0x0806F800
    #define ADDR_TIME_SAFETY            0x08070000
    #define ADDR_CTRL_OXY_RUN_FREE      0x08070800
    #define ADDR_PASSWORD_SETTING       0x08071000
    #define ADDR_TIME_CONTROL_SETTING   0x08071800
    #define ADDR_OXY_THRESH_SETTING     0x08072000
    #define ADDR_INFOR_SLAVE_RS485      0x08072800
    #define ADDR_INIT_STATE_MACHINE     0X08073000

    /* ============= Add Modem config ======== */
    #define	ADDR_SERVER_INFOR			0x08073800
    #define	ADDR_SERVER_BACKUP			0x08074000
    #define	ADDR_FREQ_ACTIVE    	    0x08074800
    #define ADDR_DCUID         			0x08075000  

    /* ============= Add Register Machine ========*/
    #define ADDR_PH_THRESH_SETTING      0x08075800
    #define ADDR_PH_TIME_CONTROL        0x08076000
    #define ADDR_PH_CHOOSE_OPTION       0x08076800 
    #define ADDR_PH_STATE_CTRL          0x08077000
    #define ADDR_NUMBER_MOTOR_SLAVE     0x08077800
    /*======= Define Value===========*/   
    #define	FLASH_MAX_MESS_TSVH_SAVE	30
    #define	FLASH_MAX_MESS_EVENT_SAVE   30
    #define FLASH_MAX_MESS_LOG_SAVE     128

    #define MAX_SIZE_FIRMWARE           150     //Kb
    #define FIRMWARE_SIZE_PAGE	        MAX_SIZE_FIRMWARE*1024/FLASH_PAGE_SIZE

    #define SIZE_DATA_TSVH	            128        
    #define SIZE_DATA_EVENT	            64         
    #define SIZE_DATA_LOG	            64   

#endif



#if defined (STM32L072xx) || defined (STM32L082xx)
    #define ON_FLASH_BASE               0x08000000
    #define ADDR_FLASH_END              0x08030000  

    /*========Add Firmware: Main + Update=========== */
#ifdef DEVICE_TYPE_GATEWAY
    #define	ADDR_MAIN_PROGRAM			0x08004000	        //42 Page cho chuong trinh chinh
    #define	ADDR_UPDATE_PROGRAM			0x08019000	        //42 page -> End Program = 0x0802E000

    #define ADDR_FLAG_HAVE_NEW_FW       0x0802E080

    /*========Add Record Mess ================ */
    #define	ADDR_MESS_A_START   		0x0802E000	         //3*256k -> Max 6 record
    #define	ADDR_MESS_A_STOP			0x0802E300

    #define	ADDR_EVENT_START   		    0x0802E300	         //1*256k -> Max 4 record
    #define	ADDR_EVENT_STOP			    0x0802E400

    #define	ADDR_LOG_START   		    0x0802E400	         //3*256k -> Max 4 record
    #define	ADDR_LOG_STOP			    0x0802E500           
#else
    #define	ADDR_MAIN_PROGRAM			0x08004000
    #define	ADDR_UPDATE_PROGRAM			0x0800D000	  
    
    #define ADDR_FLAG_HAVE_NEW_FW       0x0800F000

    /*========Add Record Mess ================ */
    #define	ADDR_MESS_A_START   		0x08010000 	         
    #define	ADDR_MESS_A_STOP			0x0801F000

    #define	ADDR_EVENT_START   		    0x08020000	        
    #define	ADDR_EVENT_STOP			    0x08025000

    #define	ADDR_LOG_START   		    0x08026000	         
    #define	ADDR_LOG_STOP			    0x0802C000           //6 sector = 6*4096/64 = 384 record     
#endif

    /*=======Add Index Mess================ */
    #define	ADDR_INDEX_LOG_SEND 		0x0802E680
    #define	ADDR_INDEX_LOG_SAVE 	    0x0802E700

    #define	ADDR_INDEX_EVENT_SEND 		0x0802E780
    #define	ADDR_INDEX_EVENT_SAVE 	    0x0802E800

    #define	ADDR_INDEX_TSVH_SEND 		0x0802E880
    #define	ADDR_INDEX_TSVH_SAVE 		0x0802E900

    /* ============= Addr Meter config======== */
    #define	ADDR_FLAG_RESET		        0x0802ED00
    #define	ADDR_METER_NUMBER			0x0802ED80

    #define	ADDR_LEVEL_CALIB			0x0802EE00	        
    #define	ADDR_THRESH_MEAS			0x0802EE80	

    /* ============= Addr Modem config ======== */
    #define	ADDR_SERVER_INFOR			0x0802F500
    #define	ADDR_SERVER_BACKUP			0x0802F580
    #define	ADDR_FREQ_ACTIVE    	    0x0802F600
    #define ADDR_DCUID         			0x0802F680       

#ifdef DEVICE_TYPE_GATEWAY
    #define	FLASH_MAX_MESS_TSVH_SAVE	2
    #define	FLASH_MAX_MESS_EVENT_SAVE   2
    #define FLASH_MAX_MESS_LOG_SAVE     2

    #define SIZE_DATA_TSVH	            128        
    #define SIZE_DATA_EVENT	            64         
    #define SIZE_DATA_LOG	            64    

    #define MAX_SIZE_FIRMWARE           84     //Kb
    #define FIRMWARE_SIZE_PAGE	        MAX_SIZE_FIRMWARE*1024/FLASH_PAGE_SIZE
#else
    #define	FLASH_MAX_MESS_TSVH_SAVE	30
    #define	FLASH_MAX_MESS_EVENT_SAVE   30
    #define FLASH_MAX_MESS_LOG_SAVE     128

    #define SIZE_DATA_TSVH	            128        
    #define SIZE_DATA_EVENT	            64         
    #define SIZE_DATA_LOG	            64    

    #define MAX_SIZE_FIRMWARE           84     //Kb
    #define FIRMWARE_SIZE_PAGE	        MAX_SIZE_FIRMWARE*1024/FLASH_PAGE_SIZE
#endif

#endif



/*============== Inline Flash ===================*/
#define _ON_FLASH_IS_NEW_PAGE(address)     \
                    ((address - ON_FLASH_BASE) % FLASH_PAGE_SIZE)

                           
typedef enum
{
	_EVENT_ON_FLASH_WRITE_BUFF = 0,
	_EVENT_ON_FLASH_READ_BUFF,
    _EVENT_ON_FLASH_CHECK_QUEUE,

	_EVENT_ON_FLASH_END, // don't use
}eEVENT_ON_FLASH;



extern sEvent_struct        sEventOnChipFlash[];
extern sMemoryManager       sOnFlash; 

extern Struct_Queue_Type    qOnFlashRead;
extern Struct_Queue_Type    qOnFlashWrite;


/*======================== Function ======================*/

void        OnFlash_Init (void);
uint8_t     OnFlash_Task (void);    
//Function Init
void        Flash_Get_Infor (uint32_t AddStart, uint8_t *pData, uint16_t *Length, uint8_t MAX_LEGNTH_INFOR);

uint8_t     OnFlash_Status (void);
void        OnFlash_Setup_Init (void);
uint8_t     OnFlash_Test_Write (void);

uint8_t     OnFlash_Is_Queue_Read_Empty (void);
uint8_t     OnFlash_Is_Queue_Write_Empty (void);

void        OnFlash_Send_To_Queue_Read (sQueueMemReadManager *qRead);
void        OnFlash_Send_To_Queue_Write (sQueueMemWriteManager *qWrite);

#endif