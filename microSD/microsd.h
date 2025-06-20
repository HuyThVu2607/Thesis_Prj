/*
 *  microsd.h
 *
 *  Created on: 20-June-2025
 *  Author: Huy Vu
 */

#ifndef __MICROSD_H__
#define __MICROSD_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "main.h"

//Define select mode debug
#define FAT_DEV_MODE 0
#define SD_DEV_MODE 0

void readBlockSingle(uint32_t blockIndex , uint8_t *buffAddr) ;
void writeBlockSingle(uint32_t blockIndex , uint8_t *buffAddr) ;
void SD_init();
void deselectSDcard();
void selectSDcard();

#endif /* __MICROSD_H__ */