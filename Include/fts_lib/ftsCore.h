/*

**************************************************************************
**                        STMicroelectronics							**
**************************************************************************
**                        marco.cali@st.com								**
**************************************************************************
*                                                                        *
*							FTS Core definitions						 *
*                                                                        *
**************************************************************************
**************************************************************************

*/

/*!
* \file ftsCore.h
* \brief Contains all the definitions and structs of Core functionalities
*/

#include "ftsHardware.h"
#include "ftsSoftware.h"

/*HW DATA*/
#define GPIO_NOT_DEFINED					-1									/*value assumed by reset_gpio when the reset pin of the IC is not connected*/

#define ADDR_SIZE_HW_REG					4						       	/*value of AddrSize for Hw register in FTI @see AddrSize*/

#define DATA_HEADER							4								/*size in byte of the header loaded with the data in the frambuffer*/
#define LOCKDOWN_CODE_RETRY  				2
/**
 * Type of CRC errors
 */
typedef enum {
    CRC_CODE = 1,																/*CRC in the code section*/
    CRC_CONFIG = 2,															/*CRC in the config section*/
    CRC_CX = 3,																/*CRC in the cx section*/
    CRC_PANEL = 4																/*CRC in the panel section*/
} CRC_Error;

/*CHIP INFO*/
/** @defgroup system_info	System Info
* System Info Data collect the most important informations about hw and fw
* @{
*/
#define SYS_INFO_SIZE						208									/*Size in bytes of System Info data*/
#define DIE_INFO_SIZE						16									/*num bytes of die info*/
#define EXTERNAL_RELEASE_INFO_SIZE			8									/*num bytes of external release in config*/
#define RELEASE_INFO_SIZE					(EXTERNAL_RELEASE_INFO_SIZE + 8)		/*num bytes of release info in sys info (first bytes are external release)*/
/** @}*/

/*RETRY MECHANISM*/
#define RETRY_MAX_REQU_DATA					2								/*Max number of attemps performed when requesting data*/
#define RETRY_SYSTEM_RESET					3									/*Max number of attemps performed to reset the IC*/

/*LOCKDOWN INFO*/
#define LOCKDOWN_LENGTH						384
#define LOCKDOWN_HEAD_LENGTH				4
#define LOCKDOWN_DATA_OFFSET				20
#define LOCKDOWN_SIGNATURE					0x5A
#define ADDR_LOCKDOWN						((u64)0x0000000000000000)
#define LOCKDOWN_WRITEREAD_CMD				0xA6