#include "SPI.h"

namespace Ispi
{
    esp_err_t Spi::Init(const spi_host_device_t spi_peripheral, const int pin_miso, const int pin_mosi, const int pin_sclk)
    {
        esp_err_t status = ESP_OK;

        _spi_peripheral = spi_peripheral;
        spi_bus_config_t spi_cfg{
            .mosi_io_num = pin_mosi,
            .miso_io_num = pin_miso,
            .sclk_io_num = pin_sclk,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 0,
            .flags = 0,
            .intr_flags = 0};

            status |= spi_bus_initialize(spi_peripheral, &spi_cfg, SPI_DMA_CH_AUTO);

            return status;
    }

    esp_err_t Spi::RegisterDevice(const uint8_t spi_mode, const int ss)
    {
        esp_err_t status = ESP_OK;

        spi_device_interface_config_t interface_cfg{
            .command_bits = 0, // Will not use dedicated command bits for this usecase
            .address_bits = 8, // For this usecase adding a 0 or 1 for read or write is simple enough
            .dummy_bits = 0,
            .mode = spi_mode,
            .duty_cycle_pos = 0,
            .cs_ena_pretrans = 0,
            .cs_ena_posttrans = 0,
            .clock_speed_hz = 80, // 80MHz/80 = 1MHz SPI speed
            .input_delay_ns = 0,
            .spics_io_num = ss, // IO pin 5 is my chip select
            .flags = 0,
            .queue_size = 1};

        status |= spi_bus_add_device(_spi_peripheral, &interface_cfg, &_handle);

        return status;
    }

    uint8_t Spi::ReadRegister(uint8_t reg_addr)
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

        spi_device_transmit(_handle, &_spi_transaction);

        return _spi_transaction.rx_data[0];
    }

    esp_err_t Spi::WriteRegister(uint8_t reg_addr, uint8_t reg_data)
    {
        uint8_t temp = reg_addr | 0x80;

        esp_err_t status{ESP_OK};

        spi_transaction_t _spi_transaction{
            .flags = SPI_TRANS_USE_TXDATA,
            .cmd = 1,           // Read, Not used
            .addr = temp,    // Ensure write operation
            .length = 8,
            .rxlength = 0,      // 0 defaults to length
            .user = 0,
            //.tx_buffer = NULL,
            .tx_data = {reg_data, 0, 0, 0},
            //.rx_buffer = NULL,
            .rx_data = {0}};

           status = spi_device_transmit(_handle, &_spi_transaction);

        return status;
    }

} // namespace Ispi