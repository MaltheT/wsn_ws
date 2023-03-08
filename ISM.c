#include "contiki.h"
#include <math.h>
#include <stdio.h> /* For printf() */


#include <os/dev/radio.h>  
#include "net/netstack.h"



/*---------------------------------------------------------------------------*/
PROCESS(ISM_process, "ISM process");
AUTOSTART_PROCESSES(&ISM_process);

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ISM_process, ev, data)
{
    PROCESS_BEGIN();

    int number_off_test = 10;

    static struct etimer et;
    
    etimer_set(&et, 2 * CLOCK_SECOND);

    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
 
    printf("start \n");

    radio_value_t RSSI;

    NETSTACK_RADIO.on();

    static int sum, i, j = 0;    

    for(i = 0; i < 16; i++)
    {
        if(NETSTACK_RADIO.set_value(RADIO_PARAM_CHANNEL, 11 + i) != RADIO_RESULT_OK) 
        {
            printf("failed channel set");
        }

        sum = 0;

        printf("Channel: %d \n", i + 11);

        for(j = 0; j < number_off_test; j++)
        {

            etimer_reset(&et);

            if(NETSTACK_RADIO.get_value(RADIO_PARAM_RSSI, &RSSI) != RADIO_RESULT_OK) 
            {
                printf("failed get RSSI value");
            }

            sum += RSSI;

            printf("%d \n", RSSI);

            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

        }
        
        printf("SUM of RSSI: %d \n\n", sum / number_off_test);

    }
    
    printf("stop \n");

    PROCESS_END();
}