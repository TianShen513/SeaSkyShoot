#include "task_lightSensor.h"

QueueHandle_t Queue_LightSensor_Meas;
SemaphoreHandle_t Sem_I2c;

void init_task_lightSensor()
{
    // create the queue for light sensor measurement
    Queue_LightSensor_Meas = xQueueCreate(1, sizeof(uint16_t));
    Sem_I2c = xSemaphoreCreateBinary();
    xSemaphoreGive(Sem_I2c);
    // create the task
    BaseType_t rslt = xTaskCreate(task_lightSensor,
                                  "Light Sensor",
                                  configMINIMAL_STACK_SIZE,
                                  NULL,
                                  2,
                                  NULL);
    if (rslt == pdPASS)
    {
        printf("Light Sensor task created\n\r");
    }
    else
    {
        printf("Light Sensor task NOT created\n\r");
    }
}

void task_lightSensor(void *pvParameters)
{

    /* Allocate any local variables used in this task */
    uint8_t contr_command = 0x01;
    uint8_t meas_command = 0x12;
    uint8_t als_data_ch1[2] = {0, 0};
    uint8_t als_data_ch0[2] = {0, 0};
    uint8_t new_data_status;
    uint8_t data_status;
    uint16_t als_ch1_adc_data;
    uint16_t als_ch0_adc_data;
    uint32_t als_data = 0;

    while (1)
    {
        // take semaphore to make sure LED IO Expander can't use I2C
        xSemaphoreTake(Sem_I2c, portMAX_DELAY);
        // constantly check the sensor measuremnt
        io_expander_set_contr_port(contr_command); // gain 0x1

        // Set ALS Integration Time 200ms, Repeat Rate 200ms
        io_expander_set_meas_port(meas_command);

        als_data_ch1[0] = io_read_reg(LTR_ALS_DATA_CH1_0);
        als_data_ch1[1] = io_read_reg(LTR_ALS_DATA_CH1_1);
        als_data_ch0[0] = io_read_reg(LTR_ALS_DATA_CH0_0);
        als_data_ch0[1] = io_read_reg(LTR_ALS_DATA_CH0_1);

        // Place the bytes of als_data_ch1 into the higher 16 bits of als_data
        als_data |= ((uint32_t)als_data_ch1[0] << 24);
        als_data |= ((uint32_t)als_data_ch1[1] << 16);

        // Place the bytes of als_data_ch0 into the lower 16 bits of als_data
        als_data |= ((uint32_t)als_data_ch0[0] << 8);
        als_data |= (uint32_t)als_data_ch0[1];

        als_ch1_adc_data = (als_data_ch1[1] << 8) | als_data_ch1[0];
        als_ch0_adc_data = (als_data_ch0[1] << 8) | als_data_ch0[0];

        data_status = io_read_reg(LTR_ALS_STATUS);
        xSemaphoreGive(Sem_I2c);
        new_data_status = data_status & 0x04;
        //printf("new_data_status is value at %d \n\r", new_data_status);
        if(new_data_status == 0x00){
       
             xQueueSend(Queue_LightSensor_Meas, &als_ch0_adc_data, portMAX_DELAY);
        }

       

    }
}