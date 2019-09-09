/*******************************************************************************
* COPYRIGHT  CO.,LTD
********************************************************************************
* �ļ�����:  drv_pub_type.h
* ��������:
* ʹ��˵��:  ��
* �ļ�����:  2015/2/3
* ��д����:  2015/2/3
* �޸���ʷ:
* �޸İ汾  �޸�����       �޸���   �޸�����
* ------------------------------------------------------------------------------
  01a       2015/2/3        ���վ�   ����
*******************************************************************************/

/******************************** ͷ�ļ�������ͷ *****************************/
#ifndef __PUB_TYPE_H /* ͷ�ļ����� */
#define __PUB_TYPE_H /* ͷ�ļ����� */

/******************************** ���Ͷ��� ***********************************/
#ifndef DRV_OK
#define DRV_OK         (0)    /* �ɹ� */
#endif

#ifndef DRV_ERROR
#define DRV_ERROR      (-1)   /* ʧ�� */
#endif

#ifndef OK
#define OK         (0)    /* �ɹ� */
#endif

#ifndef ERROR
#define ERROR      (-1)   /* ʧ�� */
#endif

#ifndef NULLPTR
#define NULLPTR ((void *)0)
#endif

#ifndef FALSE
#define FALSE          0
#endif

#ifndef TRUE
#define TRUE           1
#endif

#if  ((!(defined(TRUE))) || (TRUE!=1))
#define TRUE    1
#endif

#undef NULL
#define NULL ((void *)0)

typedef long                LONG;
typedef unsigned long       ULONG;
typedef	char		        INT8;
typedef	short		        INT16;
typedef	int		            INT32;
typedef	long    	        INT64;
typedef	unsigned char	    UINT8;
typedef	unsigned short	    UINT16;
typedef	unsigned int	    UINT32;
typedef	unsigned long long  UINT64;
typedef	int		            BOOL;
typedef	int		            STATUS;
typedef void	            VOID;
typedef char                CHAR;
typedef char*               STRING;


typedef	char		        s8;
typedef	short		        s16;
typedef	int		            s32;
typedef	long    	        s64;
typedef	unsigned char	    u8;
typedef	unsigned short	    u16;
typedef	unsigned int	    u32;
typedef	unsigned long long  u64;

typedef volatile char                VCHAR;
typedef	volatile char		         VINT8;
typedef	volatile short		         VINT16;
typedef	volatile int		         VINT32;
typedef	volatile long    	         VINT64;
typedef	volatile unsigned char	     VUINT8;
typedef	volatile unsigned short	     VUINT16;
typedef	volatile unsigned int	     VUINT32;
typedef	volatile unsigned long long  VUINT64;

//typedef	char		        int8_t;
typedef	short		        int16_t;
typedef	int		            int32_t;
//typedef	long    	        int64_t;
typedef	unsigned char	    uint8_t;
typedef	unsigned short	    uint16_t;
typedef	unsigned int	    uint32_t;
//typedef	unsigned long long  uint64_t;

typedef	int	    status_t;

typedef INT32 (*pTr)(void);  

/******************************** ͷ�ļ�������β *****************************/
#endif /* __PUB_TYPE_H *//* ͷ�ļ����� */
/******************************** ͷ�ļ����� *********************************/
