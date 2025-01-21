# ESP idf
## I. Giới thiệu chung
### 1. Documents
- Websize : [espressif](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/index.html)
- schematic :
- reference manual:
- datasheet :
- software : [esp-idf v4.2.3](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/windows-setup.html) + [visual studio code](https://code.visualstudio.com/) 
<!-- `` note `` [video hướng dẫn cài esp-idf](https://www.youtube.com/watch?v=YK9g9GWQigg&t=146s) -->
### 2. Tạo mới một project
- coppy một project mẫu
- thay đổi tên dự án, tên file .c 
- ./main/CMakeLists.txt thay đổi: idf_component_register(SRCS "tên_hàm_main.c"INCLUDE_DIRS ".")
- ./CMakeLists.txt thay đổi: project(tên_project)
- ./Makefile.mk PROJECT_NAME := sample_project
### 4. Tạo và thêm thư viện
- Tạo folder chứa thư viện và header file (*.h) + source file (*.c) + CMakeList.txt + component.mk
- ./lib/CMakeList.txt thêm
``` Makefile
set(app_src tên_source.c)
set(pri_req )
idf_component_register(SRCS ${app_src}
                    INCLUDE_DIRS "."
                    REQUIRES ${pri_req})
```
- ./lib/component.mk thêm
``` Makefile
COMPONENT_ADD_INCLUDEDIRS := .
COMPONENT_SRCDIRS := .
```
- ./CMakeLists.txt
``` Makefile
set(EXTRA_COMPONENT_DIRS ${CMAKE_CURENT_LIST_DIR}/tên_folder_chứa_thư_viện)
```
### 3. Build và load code to tagert
- command line 
``` Shell
cd PATH             # PATH đường dẫn đến folder chứa project
idf.py buildconfig  # tùy chỉnh cấu hình trong khi build
idf.py build        # buil chương trình
idf.py -p (PORT) flash monitor # Port: cổng kết nối và nạp chương trình vào esp32
```
## II. GPIO API
``` C
// cấu hình chân io
typedef struct {
    uint64_t pin_bit_mask;          /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
    gpio_mode_t mode;               /*!< GPIO mode: set input/output mode                     */
    gpio_pullup_t pull_up_en;       /*!< GPIO pull-up                                         */
    gpio_pulldown_t pull_down_en;   /*!< GPIO pull-down                                       */
    gpio_int_type_t intr_type;      /*!< GPIO interrupt type                                  */
} gpio_config_t;

sp_err_t gpio_config(const gpio_config_t *pGPIOConfig);

/* đọc giá trị từ pin */
int gpio_get_level(gpio_num_t gpio_num);

/* đặt giá trị cho pin */
esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level);

/* tải xuống isr từ driver */
esp_err_t gpio_install_isr_service(int intr_alloc_flags);

/* sử dụng API này sau khi gọi gpio_install_isr_service
    @param[isr_handler] : con trỏ hàm được dùng để xử lý ngắt
    @param[args] : tham số truyền vào hàm xử lý ngắt mẫu  */
esp_err_t gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args);

/* hàm xử lý ngắt mẫu */
static void IRAM_ATTR isr_handler(void* arg)
{
    // code
}
```


