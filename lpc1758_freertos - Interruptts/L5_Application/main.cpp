/*
 *     SocialLedge.com - Copyright (C) 2013
 *
 *     This file is part of free software framework for embedded processors.
 *     You can use it and/or distribute it as long as this copyright header
 *     remains unmodified.  The code is free for personal use and requires
 *     permission to use in a commercial product.
 *
 *      THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 *      OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 *      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 *      I SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 *      CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *     You can reach the author of this software at :
 *          p r e e t . w i k i @ g m a i l . c o m
 */

/**
 * @file
 * @brief This is the application entry point.
 * 			FreeRTOS and stdio printf is pre-configured to use uart0_min.h before main() enters.
 * 			@see L0_LowLevel/lpc_sys.h if you wish to override printf/scanf functions.
 *
 */
#include "tasks.hpp"
#include "examples/examples.hpp"
#include<stdio.h>
#include "printf_lib.h"
#include "my_eint3_interrupt.h"


void mynewf1(void)
{
	u0_dbg_printf("\n switch 1 created \n");
}
void mynewf2(void)
{
	u0_dbg_printf("\n switch 2 created \n");
}

void mynewf3(void)
{
	u0_dbg_printf("\n switch 3 created\n");
}
void mynewf4(void)
{
	u0_dbg_printf("\n switch 4 created\n");
}
void mynewf5(void)
{
	u0_dbg_printf("\n switch 5 created\n");
}




/**
 * The main() creates tasks or "threads".  See the documentation of scheduler_task class at scheduler_task.hpp
 * for details.  There is a very simple example towards the beginning of this class's declaration.
 *
 * @warning SPI #1 bus usage notes (interfaced to SD & Flash):
 *      - You can read/write files from multiple tasks because it automatically goes through SPI semaphore.
 *      - If you are going to use the SPI Bus in a FreeRTOS task, you need to use the API at L4_IO/fat/spi_sem.h
 *
 * @warning SPI #0 usage notes (Nordic wireless)
 *      - This bus is more tricky to use because if FreeRTOS is not running, the RIT interrupt may use the bus.
 *      - If FreeRTOS is running, then wireless task may use it.
 *        In either case, you should avoid using this bus or interfacing to external components because
 *        there is no semaphore configured for this bus and it should be used exclusively by nordic wireless.
 */


void GPIO_SM_init()
{
	port2_enable_eint3_interrupt(1,mynewf1);
	port2_enable_eint3_interrupt(0,mynewf2);
	port2_enable_eint3_interrupt(2,mynewf3);
	port0_enable_eint3_interrupt(1,mynewf4);
	port0_enable_eint3_interrupt(0,mynewf5);
}



void button_press_task(void *p)
{
	while(1)
	{
		puts("Hey");
		vTaskDelay(1000);
	}
}





class InterruptTask: public scheduler_task
{
public:
	InterruptTask(uint8_t priority) :scheduler_task("InterruptTask", 2000, priority)
{

}
	bool init(void){


		uint8_t pin,f;


		u0_dbg_printf("Choose  Port 2 pin number \n");
		scanf("%d",&pin);
		u0_dbg_printf(" pin number %d chosen \n",pin);
		u0_dbg_printf("Choose function 1,2,3,4 or 5\n");

		scanf("%d",&f);
		u0_dbg_printf(" function %d chosen \n",f);
		switch(f)
		{
		case 1:
			port2_enable_eint3_interrupt(pin,mynewf1);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 2:
			port2_enable_eint3_interrupt(pin,mynewf2);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 3:
			port2_enable_eint3_interrupt(pin,mynewf3);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 4:
			port2_enable_eint3_interrupt(pin,mynewf4);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 5:
			port2_enable_eint3_interrupt(pin,mynewf5);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		default:
			u0_dbg_printf("Incorrect function chosen\n");
			break;

		}


		u0_dbg_printf("Choose Port 0 pin number \n");
		scanf("%d",&pin);
		u0_dbg_printf(" pin number %d chosen \n",pin);
		u0_dbg_printf("Choose function 1,2,3,4 or 5\n");

		scanf("%d",&f);
		u0_dbg_printf(" function %d chosen \n",f);
		switch(f)
		{
		case 1:
			port0_enable_eint3_interrupt(pin,mynewf1);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 2:
			port0_enable_eint3_interrupt(pin,mynewf2);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 3:
			port0_enable_eint3_interrupt(pin,mynewf3);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 4:
			port0_enable_eint3_interrupt(pin,mynewf4);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 5:
			port0_enable_eint3_interrupt(pin,mynewf5);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		default:
			u0_dbg_printf("Incorrect function chosen\n");
			break;

		}

		return true;
	}

	bool run(void *p)
	{
		u0_dbg_printf(" Hello ");
		vTaskDelay(1000);
		return true;
	}

};


int main(void)
{
	/**
	 * A few basic tasks for this bare-bone system :
	 *      1.  Terminal task provides gateway to interact with the board through UART terminal.
	 *      2.  Remote task allows you to use remote control to interact with the board.
	 *      3.  Wireless task responsible to receive, retry, and handle mesh network.
	 *
	 * Disable remote task if you are not using it.  Also, it needs SYS_CFG_ENABLE_TLM
	 * such that it can save remote control codes to non-volatile memory.  IR remote
	 * control codes can be learned by typing the "learn" terminal command.
	 */
#if 0
	while(1){
		uint8_t pin,f;
		u0_dbg_printf("Choose pin number \n");
		scanf("%d",&pin);
		u0_dbg_printf(" pin number %d chosen \n",pin);
		u0_dbg_printf("Choose function 1,2,3,4 or 5\n");
		scanf("%d",&f);
		u0_dbg_printf(" function %d chosen \n",f);
		switch(f)
		{
		case 1:
			port2_enable_eint3_interrupt(pin,mynewf1);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 2:
			port2_enable_eint3_interrupt(pin,mynewf2);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 3:
			port2_enable_eint3_interrupt(pin,mynewf3);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 4:
			port2_enable_eint3_interrupt(pin,mynewf4);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		case 5:
			port2_enable_eint3_interrupt(pin,mynewf5);
			u0_dbg_printf("Function %d chosen pin number %d\n",f,pin);
			break;
		default:
			u0_dbg_printf("Incorrect function chosen\n");
			break;

		}
	}

#endif

#if 1
	scheduler_add_task(new InterruptTask(PRIORITY_HIGH));
#endif

#if 0
	//gButtonPressSemaphore = xSemaphoreCreateBinary();
	GPIO_SM_init();
	xTaskCreate(button_press_task, "button task",2048,NULL,PRIORITY_HIGH,NULL);
#endif


	//	scheduler_add_task(new terminalTask(PRIORITY_HIGH));

	/* Consumes very little CPU, but need highest priority to handle mesh network ACKs */
	scheduler_add_task(new wirelessTask(PRIORITY_CRITICAL));

	/* Change "#if 0" to "#if 1" to run period tasks; @see period_callbacks.cpp */
#if 0
	scheduler_add_task(new periodicSchedulerTask());
#endif

	/* The task for the IR receiver */
	// scheduler_add_task(new remoteTask  (PRIORITY_LOW));

	/* Your tasks should probably used PRIORITY_MEDIUM or PRIORITY_LOW because you want the terminal
	 * task to always be responsive so you can poke around in case something goes wrong.
	 */

	/**
	 * This is a the board demonstration task that can be used to test the board.
	 * This also shows you how to send a wireless packets to other boards.
	 */
#if 0
	scheduler_add_task(new example_io_demo());
#endif

	/**
	 * Change "#if 0" to "#if 1" to enable examples.
	 * Try these examples one at a time.
	 */
#if 0
	scheduler_add_task(new example_task());
	scheduler_add_task(new example_alarm());
	scheduler_add_task(new example_logger_qset());
	scheduler_add_task(new example_nv_vars());
#endif

	/**
	 * Try the rx / tx tasks together to see how they queue data to each other.
	 */
#if 0
	scheduler_add_task(new queue_tx());
	scheduler_add_task(new queue_rx());
#endif

	/**
	 * Another example of shared handles and producer/consumer using a queue.
	 * In this example, producer will produce as fast as the consumer can consume.
	 */
#if 0
	scheduler_add_task(new producer());
	scheduler_add_task(new consumer());
#endif

	/**
	 * If you have RN-XV on your board, you can connect to Wifi using this task.
	 * This does two things for us:
	 *   1.  The task allows us to perform HTTP web requests (@see wifiTask)
	 *   2.  Terminal task can accept commands from TCP/IP through Wifly module.
	 *
	 * To add terminal command channel, add this at terminal.cpp :: taskEntry() function:
	 * @code
	 *     // Assuming Wifly is on Uart3
	 *     addCommandChannel(Uart3::getInstance(), false);
	 * @endcode
	 */
#if 0
	Uart3 &u3 = Uart3::getInstance();
	u3.init(WIFI_BAUD_RATE, WIFI_RXQ_SIZE, WIFI_TXQ_SIZE);
	scheduler_add_task(new wifiTask(Uart3::getInstance(), PRIORITY_LOW));
#endif

	scheduler_start(); ///< This shouldn't return
	return -1;
}
