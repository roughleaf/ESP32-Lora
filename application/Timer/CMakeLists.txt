set(SOURCES AppTimer.cpp)

idf_component_register(SRCS ${SOURCES}
                        INCLUDE_DIRS .
                        REQUIRES driver Gpio esp_event)