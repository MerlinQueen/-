#include "sys.h"
#include "FDC2X2X_CMD.h"

#ifndef FDC2214_H_
#define FDC2214_H_





void FDC2214_Init(void);	//��ʼ��FDC2214��I2C
float FDC2214_read_CHx(u8 CHx);	//��Channel x Conversion Result





#endif
