#ifndef _LIBRARY_H_
#define _LIBRARY_H_
/*******************************************************************************
* Define
*******************************************************************************/
typedef enum
{
    START_CODE = 0,
    BYTE_COUNT,
    ADDRESS,
    RECORD_TYPE,
    DATA,
    CHECK_SUM,
    DONE
} line_action_e;

enum check_sum_e /* check sum */
{
    CHECK_SUM_SUCCESS = 0,
    CHECK_SUM_FALSE
};

enum check_line_e /* check line */
{
    CHECK_LINE_SUCCESS = 0,
    CHECK_LINE_FALSE
};

enum check_hex_e /* check hex */
{
    CHECK_HEX_SUCCESS = 0,
    CHECK_HEX_FALSE
};

enum check_record_type_e /* record type */
{
    CHECK_RECORD_TYPE_SUCCESS = 0,
    CHECK_RECORD_TYPE_FALSE
};

typedef struct /* form data line */
{
    uint8_t byte_count;
    uint8_t address[2];
    uint8_t record_type;
    uint8_t data[255];
    uint8_t check_sum;
} form_data_t;

/*******************************************************************************
* API
*******************************************************************************/

/**
 * @brief Check characters on 1 line
 *
 * @param[in] data Data to check
 * @return uint8_t CHECK_LINE_SUCCESS
 *                 CHECK_LINE_FALSE
 */
uint8_t check_line(uint8_t *data);


/**
 * @brief check record type
 *
 * @param[in] hex_t record type
 * @return uint8_t CHECK_RECORD_TYPE_SUCCESS
 *                 CHECK_RECORD_TYPE_FALSE
 */
uint8_t check_record_type(form_data_t hex_t);

/**
 * @brief convert string to hex
 *
 * @param[in] data data convert
 * @return uint8_t hex number
 */
uint8_t convert_string_to_hex(uint8_t *data);

/**
 * @brief check data to display on screen
 *
 * @param[in] data number to display
 */
void check_data(uint8_t data);

/**
 * @brief Read line by line
 *
 * @param[in] fp pointer file
 * @param[in] data save value of 1 line
 * @return char
 */
uint8_t read_line(FILE *fp, uint8_t *data);

/**
 * @brief check sum calculation
 *
 * @param[in] hex_t bytes per line
 * @return uint8_t CHECK_SUM_SUCCESS
 *                 CHECK_SUM_FALSE
 */
uint8_t check_sum(form_data_t hex_t);


/**
 * @brief Display all information of 1 line
 *
 * @param[in] hex_t  bytes per line
 * @param[in_out] flags flag remember case state record type 0x04
 * @param[in_out] address  value of data
 * @param[in_out] flags_2 flag remember case state record type 0x02
 * @param[in_out] address_2 value of data
 */
void display(form_data_t hex_t, uint8_t *flags, uint8_t address[], uint8_t *flags_2, uint8_t address_2[]);

#endif /* _LIBRARY_H_ */

/*******************************************************************************
* End of file
*******************************************************************************/
