/*******************************************************************************
*  COPYRIGHT BLUEBAY TECH CO.,LTD
********************************************************************************
* �ļ�����:  api.h
* ��������:
* ʹ��˵��:  ��
* �ļ�����:  LIPUJUN
* ��д����:  2018/11/28
* �޸���ʷ:
* �޸İ汾  �޸�����       �޸���   �޸�����
* ------------------------------------------------------------------------------
  01a       2018/11/28        ���վ�   ����
*******************************************************************************/

/******************************** ͷ�ļ�������ͷ *****************************/
#ifndef __API_H /* ͷ�ļ����� */
#define __API_H /* ͷ�ļ����� */

#include "pub_type.h"

/******************************** ���Ͷ��� ***********************************/

enum _API_ERRCODE
{
    _API_ERRCODE_START = 0x10000+0x2000,
    RS232_RS422_POWER_PORT_ERRCODE = _API_ERRCODE_START,
    RS232_RS422_POWER_PD_ERRCODE,
    RS232_RS422_LOOP_PORT_ERRCODE = _API_ERRCODE_START+0x10,
    RS232_RS422_LOOP_PD_ERRCODE,
    RS232_RS422_MODE_PORT_ERRCODE = _API_ERRCODE_START+0x20,
    RS232_RS422_MODE_PD_ERRCODE,
    WIFI_POWER_PD_ERRCODE = _API_ERRCODE_START+0x30,
    RS422_POWER_PORT_ERRCODE = _API_ERRCODE_START+0x40,
    RS422_POWER_PD_ERRCODE,
    RS485_POWER_PD_ERRCODE = _API_ERRCODE_START+0x50,
    RS232_POWER_PD_ERRCODE = _API_ERRCODE_START+0x60,
    CAN_POWER_PD_ERRCODE = _API_ERRCODE_START+0x70,

    RS232_RS422_RECV_EMPTY_PORT_ERRCODE = _API_ERRCODE_START+0x80,
    RS232_RS422_RECV_EMPTY_BUF_NULL_ERRCODE,
    RS232_RS422_RECV_EMPTY_NO_ERRCODE,

};

INT32 imx6Init (void);
INT32 rs232_rs422_cfg (UINT8 port, UINT32 baudRate, UINT8 dataBit, UINT8 stopBit, UINT8 parity);
INT32 rs232_rs422_send (UINT8 port, CHAR *buf, UINT32 len);
INT32 rs232_rs422_recv (UINT8 port, CHAR *buf, UINT32 len);
INT32 rs232_rs422_recv_empty (UINT8 port, CHAR *buf);
INT32 rs232_rs422_power_set (UINT8 port, UINT8 pd);
INT32 rs232_rs422_loop_set (UINT8 port, UINT8 loop);
INT32 rs232_rs422_mode_switch (UINT8 port, UINT8 sw);
INT32 wifi_power_set (UINT8 pd);
INT32 rs422_power_set (UINT8 port, UINT8 pd);
INT32 rs485_power_set (UINT8 pd);
INT32 rs232_power_set (UINT8 pd);
INT32 can_power_set (UINT8 pd);

INT32 DPRAM_Read (UINT8 num, UINT32 offset, CHAR *buf, UINT32 len);
INT32 DPRAM_Write (UINT8 num, UINT32 offset, CHAR *buf, UINT32 len);

INT32 uartProCfg (UINT8 port, UINT16 pro, UINT8 fix, UINT16 dLen, CHAR *head, UINT16 hLen, CHAR *end, UINT16 eLen);
INT32 uartProPacketParse (UINT8 port, CHAR *buf, UINT32 *len);
UINT64 machineTimeGet (void);
INT32 UTCGet (CHAR *utc);
INT32 rs422OptCfg (UINT8 port, UINT32 hz, UINT32 duty, UINT16 edge);
INT32 isrCallbackFunReg (UINT8 isr, pTr pFunc);

/******************************** ͷ�ļ�������β *****************************/
#endif /* __API_H *//* ͷ�ļ����� */
/******************************** ͷ�ļ����� *********************************/
