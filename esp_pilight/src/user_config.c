/******************************************************************************
* Copyright 2015 liv
*
* FileName: user_config.c
*
* Description: stores, loads configuration from flash
*
*******************************************************************************/
#ifdef WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#else
#include "esp_common.h"
#endif
#include "debug.h"
#include "user_config.h"


void user_config_load (void)
{
    // load file/ flas and update all vars 
}

void user_config_save (void)
{
    // parse UC_DECALRE_ALL_VARS, serialize all vars with save flag set, and write to file/flash
}


void user_config_init (void)
{
    user_config_load ();
}


