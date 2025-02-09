set(app_src output_io.c)
set(pri_req esp_driver_gpio)
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})