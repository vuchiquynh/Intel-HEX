/*******************************************************************************
* Include
*******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "library.h"

/*******************************************************************************
* API
*******************************************************************************/

/**
 * @brief check if the key belongs to HEX
 *
 * @param[in] key keywords to check
 * @return uint8_t CHECK_HEX_SUCCESS
 *                 CHECK_HEX_FALSE
 */
static uint8_t check_hex(uint8_t key);
/*******************************************************************************
* Prototypes
*******************************************************************************/
static uint8_t check_hex(uint8_t key)
{
    uint8_t flags = CHECK_HEX_SUCCESS;

    if ( (( key >= '0') && (key <= '9')) || (( key >= 'a') && (key <= 'f')) || (( key >= 'A') && (key <= 'F')))
    {
        flags = CHECK_HEX_FALSE;
    }

    return flags;
}

uint8_t check_line(uint8_t *data)
{
    uint8_t ans;
    uint8_t flags = 0;

    if (*data != ':')
    {
        printf("->  False start code !\n");
    }
    if(strlen(data) < 11)
    {
        flags = 1;
    }
    else
    {
        for (ans = 1; ans < strlen(data); ans++) /* check key */
        {
            if(check_hex(*(data + ans)) == 0)
            {
                flags = 1;
            }
        }
    }

    return flags;
}


uint8_t check_record_type(form_data_t hex_t)
{
    uint8_t flags = CHECK_RECORD_TYPE_SUCCESS;

    if ((hex_t.record_type < 0x00) || (hex_t.record_type > 0x05))
    {
        flags = CHECK_RECORD_TYPE_FALSE;
    }

    return flags;
}

uint8_t read_line(FILE *fp, uint8_t *data)
{
    uint32_t count = 0;
    uint8_t flags = 1;
    uint8_t data_line;

    while((data_line = fgetc(fp)) != '\n') /* read line to line */
    {
        data[count] = data_line;
        count++;
    }

    return flags;
}

uint8_t convert_string_to_hex(uint8_t *data)
{
    uint8_t index = 0;
    uint8_t count = 0;
    uint8_t temp[2] = {0};
    uint8_t ans = 0;

    /* convert char array to hex */
    for( index = 0 ; index < 2; index++)
    {
        if ((data[index] >= '0') && (data[index] <= '9'))
        {
            temp[index] = data[index] - '0';
        }
        else if ((data[index] >= 'A') && (data[index] <= 'F'))
        {
            temp[index] = data[index] - 'A' + 10;
        }
        else
        {
            index = 2;
        }
        if (index == 1)
        {
            ans= ((temp[index-1]<<4) | (temp[index]));
        }
    }
    return ans;
}

void check_data(uint8_t data)
{
    if(data <= 15)
    {
        printf("0%X", data);
    }
    else
    {
        printf("%X", data);
    }
}

uint8_t check_sum(form_data_t hex_t)
{
    uint8_t sum;
    uint8_t flags = CHECK_SUM_SUCCESS;
    uint8_t ans;

    sum = hex_t.byte_count + hex_t.address[0] + hex_t.address[1] + hex_t.record_type;
    for (ans = 0; ans < hex_t.byte_count; ans++)
    {
        sum += hex_t.data[ans];
    }
    sum = ~sum + 1;  /* checksum calculation */
    if (sum == hex_t.check_sum)
    {
        printf("->  Done !\n");
    }
    else
    {
        printf("->  False check sum !\n");
        flags = CHECK_SUM_FALSE;
    }

    return flags;
}

void display(form_data_t hex_t, uint8_t *flags, uint8_t address[], uint8_t *flags_2, uint8_t address_2[])
{
    uint8_t ans = 0;
    uint32_t temp = 0;
    uint32_t temp_1 = 0;

    printf("Record type: 0x");
    check_data(hex_t.record_type);
    printf("\nData byte: ");
    for (ans = 0; ans < hex_t.byte_count; ans++)
    {
        if (*flags == 0)
        {
            address[ans] = hex_t.data[ans];
        }
        if (*flags_2 == 0)
        {
            address_2[ans] = hex_t.data[ans];
        }
        if (ans == 0)/* display data */
        {
            check_data(hex_t.data[ans]);
        }
        else
        {
            printf("-");
            check_data(hex_t.data[ans]);
        }
    }
    if ((*flags == 1) && (hex_t.record_type != 0x02))
    {   /* display record type 0x04 */
        printf("\nAddress: 0x");
        check_data(address[0]);
        check_data(address[1]);
        check_data(hex_t.address[0]);
        check_data(hex_t.address[1]);
        *flags_2 = 0;
    }
    else if ((*flags_2 == 1)&&(hex_t.record_type != 0x04))
    {   /* display record type 0x02 */
        temp = ((hex_t.address[0]<<8) | (hex_t.address[1]));
        temp_1 = ((address_2[0]<<8) | (address_2[1]));
        printf("\nAddress: 0x%X", (temp_1<<4)|temp);
        *flags = 0;
    }
    else
    {
        printf("\nAddress: 0x");
        check_data(hex_t.address[0]);
        check_data(hex_t.address[1]);
    }
    if (hex_t.record_type == 0x04)
    {
        *flags = 1;
        *flags_2 = 0;
    }
    if (hex_t.record_type == 0x02)
    {
        *flags_2 = 1;
        *flags = 0;
    }

}
/*******************************************************************************
* End of file
*******************************************************************************/
