#ifndef NUKE_DRIVER_H
#define NUKE_DRIVER_H

#include <stddef.h> // Required for size_t

/*
================================================================================
|                                                                              |
|                           模拟硬件层 (Hardware Simulation)                   |
|                                                                              |
================================================================================
*/

/**
 * @brief 模拟的GPIO硬件寄存器。
 *
 * 'extern' 表示这个变量的实体定义在别处（在 nuke_driver.c 中）。
 * 'volatile' 关键字是关键：
 *   1. 告诉编译器，这个变量的值可能在任何时候被外部因素（如硬件）改变。
 *   2. 防止编译器对该变量的读写操作进行优化（例如，缓存到CPU寄存器中）。
 * 这使得其行为更接近一个真实的内存映射I/O寄存器。
 */
extern volatile unsigned char simulated_gpio_register;


/*
================================================================================
|                                                                              |
|                         驱动接口层 (VFS Simulation)                          |
|                                                                              |
================================================================================
*/

/**
 * @brief 模拟Linux内核中的 `struct file_operations`。
 *
 * 这是一个函数指针的集合，定义了一个字符设备驱动必须提供的一套标准操作。
 * 应用程序通过VFS层调用这些标准操作，VFS再通过这些指针，调用到我们
 * 具体的驱动实现函数中。这是实现“接口与实现分离”的核心。
 */
struct nuke_driver_operations {
    /**
     * @brief 模拟 open() 系统调用。
     * @param device_name 尝试打开的设备名。
     * @return 成功时返回一个非NULL的句柄，失败时返回NULL。
     */
    void* (*open)(const char* device_name);

    /**
     * @brief 模拟 write() 系统调用。
     * @param handle open() 返回的句柄。
     * @param buffer 包含要写入数据的缓冲区。
     * @param len 要写入的字节数。
     * @return 成功写入的字节数。
     */
    int (*write)(void* handle, const char* buffer, size_t len);

    /**
     * @brief 模拟 close() 系统调用。
     * @param handle open() 返回的句柄。
     */
    void (*close)(void* handle);
};


/*
================================================================================
|                                                                              |
|                       驱动实现层 (Driver Implementation)                     |
|                                                                              |
================================================================================
*/

// --- 函数原型 (Prototypes) ---
// 这些是我们驱动的具体实现函数，它们的地址将被赋给 nuke_driver_operations 结构体。
void* nuke_open(const char* device_name);
int nuke_write(void* handle, const char* buffer, size_t len);
void nuke_close(void* handle);

/**
 * @brief 全局的驱动实例。
 *
 * 'extern' 表示这个结构体实例的实体定义在 nuke_driver.c 中。
 * 应用层代码将通过这个实例来访问驱动的功能，模拟了内核中驱动的注册过程。
 * 'const' 表示这个结构体实例本身（即它包含的函数指针）在初始化后不应被修改。
 */
extern const struct nuke_driver_operations nuke_driver;

#endif // NUKE_DRIVER_H