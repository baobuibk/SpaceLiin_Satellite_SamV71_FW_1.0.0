#include "bsp_bmp390.h"
#include "bmp390.h"
#include "stdbool.h"
#include "os_hal.h" // Để dùng vTaskDelay hoặc os_delay



bool bmp390_init(i2c_io_t* p_i2c)
{
    if (BMP390_init(p_i2c) == 1) 
        return true;
    else   
        return false;
}



/**
 * @brief Polling Read BMP390
 * @param p_i2c:  I2C (Onboard hoặc External)
 * @param p_data: pointer for receiving data
 * @return bool
 */
bool bmp390_bsp_read_polling(i2c_io_t* p_i2c, bmp390_data_t* p_data)
{
    // 1. Lấy mốc thời gian bắt đầu
    TickType_t xStartTime = xTaskGetTickCount();
    
    // 2. Định nghĩa thời gian chờ tối đa (ví dụ 150ms để dư dả cho Oversampling cao)
    const TickType_t xMaxWaitTime = pdMS_TO_TICKS(150); 

    // 3. Vòng lặp Polling
    while ((xTaskGetTickCount() - xStartTime) < xMaxWaitTime)
    {
        /* * Gọi hàm Driver bạn đã có. 
         * Hàm này sẽ tự: Trigger đo -> Check Status -> Nếu Ready thì Read & Compensate.
         */
        if (BMP390_read_value(p_i2c, p_data) == 1)
        {
            // Nếu hàm trả về 1, nghĩa là dữ liệu đã READY và đã được tính toán xong
            return true; 
        }

        /* * Nếu hàm trả về 0, nghĩa là cảm biến chưa đo xong (is_value_ready == false).
         * Chúng ta YIELD (nhường CPU) 5ms để các task khác làm việc, 
         * sau đó quay lại kiểm tra tiếp.
         */
        vTaskDelay(pdMS_TO_TICKS(5));
    }

    // Sau 150ms mà vẫn không xong thì báo lỗi Timeout
    return false;
}