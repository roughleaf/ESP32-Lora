#include "Lora.h"

namespace Lora
{
    esp_err_t Lora::Init(void)
    {
        esp_err_t status{ESP_OK};

             spi_device_interface_config_t _spi_config{
            .command_bits = 0,
            .address_bits = 8,      // Try manually controlling the control bits.
            .dummy_bits = 0,
            .mode = 0,
            .duty_cycle_pos = 0,
            .cs_ena_pretrans = 0,
            .cs_ena_posttrans = 0,
            .clock_speed_hz = 80,   // 80MHz/80 = 1MHz SPI speed
            .input_delay_ns = 0,
            .spics_io_num = 5,      // IO pin 5 is my chip select
            .flags = 0,
            .queue_size = 1}; 

        status |= spi_bus_add_device(SPI3_HOST, &_spi_config, &_spi_handle);

        return status;
    }

    uint8_t Lora::ReadRegister(uint8_t reg_addr)
    {
        uint8_t temp = reg_addr & 0x7F;

        spi_transaction_t _spi_transaction{
            .flags = SPI_TRANS_USE_RXDATA,
            .cmd = 0,     // Read, Not used
            .addr = temp, // Ensure read operation
            .length = 8,
            .rxlength = 8, // 0 defaults to length
            .user = 0,
            //.tx_buffer = NULL,
            .tx_data = {0},
            //.rx_buffer = NULL,
            .rx_data = {0}};

        spi_device_transmit(_spi_handle, &_spi_transaction);

        return _spi_transaction.rx_data[0];
    }

    esp_err_t Lora::WriteRegister(uint8_t reg_addr, uint8_t reg_data)
    {
        uint8_t temp = reg_addr & 0x7F;

        esp_err_t status{ESP_OK};

        spi_transaction_t _spi_transaction{
            .flags = SPI_TRANS_USE_TXDATA,
            .cmd = 0,           // Read, Not used
            .addr = temp,    // Ensure write operation
            .length = 8,
            .rxlength = 0,      // 0 defaults to length
            .user = 0,
            //.tx_buffer = NULL,
            .tx_data = {reg_data, 0, 0, 0},
            //.rx_buffer = NULL,
            .rx_data = {0}};

           status = spi_device_transmit(_spi_handle, &_spi_transaction);

        return status;
    }

    esp_err_t Lora::TransmitString(const char *data_tx)
    {
        esp_err_t status{ESP_OK};

        return status;
    }

} // namespace Lora