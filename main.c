/*******************************************************************************
* Include
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "library.h"

/*******************************************************************************
* Code
*******************************************************************************/
uint8_t main(uint8_t argc, uint8_t * argv [])
{
    FILE *fp = NULL;
    uint8_t data[100] = {0};
    uint8_t index = 0;
    uint8_t ans = 0;
    uint8_t sum = 0;
    uint8_t address[32] = {0};
    uint8_t address_2[32] = {0};
    form_data_t hex_t;
    line_action_e  state;
    uint8_t flags = 0;
    uint8_t flags_2 = 0;
    
    if ( argc == 1 ) 
    {
        printf( "No arguments were passed.\n" );
    }
    if ( argc == 2 ) 
    {
        fp = fopen(argv[1],"r");
        if (fp == NULL)
        {
            printf("False to open file !\n");
        }
        else
        {
            printf("Open file success !\n");
            while(read_line(fp, data) != 0)
            {
                printf("\n\n%s    ", data);
                if (check_line(data) == 0)
                {
                    state = START_CODE;
                    for ( index = 0; index < strlen(data); index++)
                    {
                        switch(state)
                        {
                            case START_CODE: /* start code*/
                                if (data[index] == ':')
                                {
                                    state = BYTE_COUNT;
                                }
                                break;
                            case BYTE_COUNT: /* byte count */
                                hex_t.byte_count = convert_string_to_hex(&data[index]);
                                index++;
                                state = ADDRESS;
                                break;
                            case ADDRESS: /* data */
                                for (ans = 0; ans < 2; ans++)
                                {
                                    hex_t.address[ans] = convert_string_to_hex(&data[index]);
                                    index += 2;
                                }
                                index--;
                                state = RECORD_TYPE;
                                break;
                            case RECORD_TYPE: /* record type */
                                hex_t.record_type  = convert_string_to_hex(&data[index]);
                                if(!check_record_type(hex_t))
                                {
                                    index++;
                                    state = DATA;
                                    if (hex_t.record_type == 0x01 ) /* check end of file*/
                                    {
                                        printf("->  end of file !\n");
                                        index = strlen(data);
                                    }
                                }
                                else
                                {
                                    printf("->  False record type !\n");
                                    index = strlen(data);
                                }
                                break;
                            case DATA: /* data */
                                if (hex_t.byte_count  == (strlen(data)-11)/2)
                                {
                                    for (ans = 0; ans < hex_t.byte_count; ans++)
                                    {
                                        hex_t.data[ans] = convert_string_to_hex(&data[index]);
                                        index += 2;
                                    }
                                    index--;
                                }
                                else
                                {
                                    printf("False data !\n");
                                }
                                state = CHECK_SUM;
                                break;
                            case CHECK_SUM: /* check sum */
                                hex_t.check_sum = convert_string_to_hex(&data[index]);
                                state = DONE;
                                break;
                            case DONE: /* end 1 line */
                                if (!check_sum(hex_t))
                                {
                                    display(hex_t, &flags, address, &flags_2, address_2);
                                }
                                index = strlen(data);
                                break;
                            default:
                                printf("->  Read false !\n");
                                break;
                        }
                    }
                }
                else
                {
                    printf("->  False line !\n");
                }
                memset(data, 0, 100);
            }
        }
    }
    
    return 0;
}

/*******************************************************************************
* End of file
*******************************************************************************/
