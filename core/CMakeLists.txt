set(SOURCES ./Src/main.cpp ./Src/EventHandlers.cpp)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS . Inc
                        REQUIRES freertos esp_common Gpio Wifi Lora SPI Timer)
