#include "SPI.h"

namespace SPI
{
    esp_err_t Spi::init(const spi_host_device_t spi_peripheral, const int pin_miso, const int pin_mosi, const int pin_sclk)
    {
        esp_err_t status = ESP_OK;

        _spi_peripheral = spi_peripheral;

        spi_transaction = {};
        spi_transaction.tx_buffer = NULL;
        spi_transaction.rx_buffer = NULL;

        spi_bus_cfg = {};       // Make sure all values is initialized to zero
        spi_bus_cfg.mosi_io_num = pin_mosi;
        spi_bus_cfg.miso_io_num = pin_miso;
        spi_bus_cfg.sclk_io_num = pin_sclk;
        spi_bus_cfg.quadwp_io_num = -1;
        spi_bus_cfg.quadhd_io_num = -1;

        status |= spi_bus_initialize(spi_peripheral, &spi_bus_cfg, SPI_DMA_CH_AUTO);

        return status;
    }

    esp_err_t Spi::registerDevice(const uint8_t spi_mode, const int ss)
    {
        esp_err_t status = ESP_OK;

        spi_interface_cfg = {};
        spi_interface_cfg.command_bits = 0; // Will not use dedicated command bits for this usecase
        spi_interface_cfg.address_bits = 8; // For this usecase adding a 0 or 1 for read or write is simple enough
        spi_interface_cfg.mode = spi_mode;
        spi_interface_cfg.clock_speed_hz = 80; // 80MHz/80 = 1MHz SPI speed
        spi_interface_cfg.spics_io_num = ss; // IO pin 5 is my chip select
        spi_interface_cfg.queue_size = 5;

        status |= spi_bus_add_device(_spi_peripheral, &spi_interface_cfg, &_handle);

        return status;
    }

    uint8_t Spi::readRegister(uint8_t reg_addr)
    {
        _transfer_byte(reg_addr & 0x7F, 0);

        return spi_transaction.rx_data[0];
    }

    esp_err_t Spi::writeRegister(uint8_t reg_addr, uint8_t reg_data)
    {
        esp_err_t status{ESP_OK};

        status |= _transfer_byte(reg_addr | 0x80, reg_data);

        return status;
    }

    esp_err_t Spi::_transfer_byte(uint8_t reg_addr, uint8_t data)
    {
        esp_err_t status { ESP_OK };

        spi_transaction.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA;
        spi_transaction.length = 8;
        spi_transaction.addr = reg_addr;
        spi_transaction.tx_data[0] = data;

        status |= spi_device_transmit(_handle, &spi_transaction);

        return status;
    }

    esp_err_t Spi::_transfer_multiples_bytes(uint8_t reg_addr, size_t data_length)
    {
        esp_err_t status { ESP_OK };
        return status;
    }

    spi_device_handle_t Spi::getHandle(void)
    {
        return _handle;
    }

} // namespace SPI