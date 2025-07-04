#include "nuke_driver.h"
#include <stdio.h>
#include <string.h> // Required for strncmp
#include <windows.h> // For SetConsoleOutputCP, CP_UTF8

// --- 模拟硬件层 (实体定义) ---
// 这里是 simulated_gpio_register 变量的实际存储位置。
volatile unsigned char simulated_gpio_register = 0x00; // 初始为低电平

// --- 驱动实现层 (函数实现) ---

/**
 * @brief nuke_open 的具体实现。
 */
void* nuke_open(const char* device_name) {
    printf("[驱动日志] 设备 '%s' 被打开。\n", device_name);
    // 在真实的驱动中，这里可能会进行硬件初始化或状态检查。
    // 我们返回一个非NULL值表示成功。这个值就是所谓的“句柄”。
    return (void*)1;
}

/**
 * @brief nuke_write 的最终实现。
 */
int nuke_write(void* handle, const char* buffer, size_t len) {
    // 显式地“使用”handle参数，以消除编译器警告。
    // 这是一种专业的编码风格，用于告知编译器和他人：我们是有意不使用这个参数的。
    (void)handle;

    if (buffer == NULL || len == 0) {
        fprintf(stderr, "[驱动日志] 错误：接收到无效的空缓冲区。\n");
        return -1; // 返回错误码
    }

    printf("[驱动日志] 接收到写入请求，数据: '%.*s', 长度: %zu\n", (int)len, buffer, len);

    // --- 核心逻辑：解析命令并操作硬件 ---
    if (len == 1 && strncmp(buffer, "1", 1) == 0) {
        printf("[驱动日志] 命令解析: SET_HIGH_LEVEL. 正在写入硬件寄存器...\n");
        simulated_gpio_register = 0x01; // 设置为高电平
    } else if (len == 1 && strncmp(buffer, "0", 1) == 0) {
        printf("[驱动日志] 命令解析: SET_LOW_LEVEL. 正在写入硬件寄存器...\n");
        simulated_gpio_register = 0x00; // 设置为低电平
    } else {
        fprintf(stderr, "[驱动日志] 警告：无法识别的命令 '%.*s'。硬件状态未改变。\n", (int)len, buffer);
    }

    // 返回成功写入的字节数
    return len;
}

/**
 * @brief nuke_close 的具体实现。
 */
void nuke_close(void* handle) {
    // 同样，显式地“使用”handle参数以消除警告。
    (void)handle;
    printf("[驱动日志] 设备被关闭。\n");
    // 在真实的驱动中，这里可能会释放资源或让硬件进入待机状态。
}

// --- 驱动接口层 (实体定义) ---
/**
 * @brief 全局驱动实例的定义和初始化。
 */
const struct nuke_driver_operations nuke_driver = {
    .open = nuke_open,
    .write = nuke_write,
    .close = nuke_close,
};