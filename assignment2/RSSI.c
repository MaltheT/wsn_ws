
/**
 * \file
 *         Program that tests the RSSI for channels 11-26  
 * \author
 *         Malthe Tøttrup <201907882@post.au.dk>
 * 
 *         $ make TARGET=sky distclean 
 *         $ make TARGET=sky MOTES=/dev/ttyUSB0 RSSI.upload login
 */

#include "contiki.h"
#include <stdio.h> /* For printf() */
#include <math.h>
#include <os/dev/radio.h>  
#include "net/netstack.h"

/*---------------------------------------------------------------------------*/
PROCESS(hello_world_process, "Hello world process");
AUTOSTART_PROCESSES(&hello_world_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(hello_world_process, ev, data)
{   
    PROCESS_BEGIN();

    static struct etimer et;
    
    etimer_set(&et, 2 * CLOCK_SECOND);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    radio_value_t RSSI;

    static int i, j, RSSI_acc, no_secs=15, min_channel=11, max_channel=26, ctr=0; //accumilating RSSI value, number of seconds for listening
    static int results[15]; for(i = 0; i < 15; i++) { results[i] = 0; } 

    NETSTACK_RADIO.on();

    for(i = min_channel; i <= max_channel; i++){
        //Connect to the desired channel
        if(NETSTACK_RADIO.set_value(RADIO_PARAM_CHANNEL, i) != RADIO_RESULT_OK) 
        {
            printf("failed channel set");
        }

        //Listen to the RSSI for 15 seconds of particular channel. Measure the RSSI every 1 second.
        RSSI_acc = 0;
        for(j = 0; j < no_secs; j++){

            etimer_set(&et, CLOCK_SECOND);                  //start 1 second timer.
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et)); 

            if(NETSTACK_RADIO.get_value(RADIO_PARAM_RSSI, &RSSI) != RADIO_RESULT_OK) 
            {
                printf("failed get RSSI value");
            }
            RSSI_acc += RSSI; //accumilate RSSI
            //printf("RSSI for channel %d = %d \n", i, RSSI);

        }    
        printf("Mean RSSI for channel %d after 15 seconds = %d \n", i, RSSI_acc/no_secs);
        results[ctr] = RSSI_acc/no_secs;
        ctr++;
    }

    //find the best channel:
    static int best = 0, best_idx = 0;
    for(i = 0; i < max_channel-min_channel; i++){
        if(results[i] < best){
            best = results[i];
            best_idx = i;
        }
    }
    printf("The channel with best RSSI is: channel %d, RSSI %d", best_idx + min_channel, best);

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
