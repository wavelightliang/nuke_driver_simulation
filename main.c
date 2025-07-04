#include "nuke_driver.h" // 包含驱动接口定义
#include <stdio.h>
#include <windows.h> // For SetConsoleOutputCP, CP_UTF8

// 辅助函数，用于打印当前模拟硬件的状态
void print_gpio_status(void);

int main() {
    // 设置控制台输出为UTF-8，确保中文日志正确显示
    SetConsoleOutputCP(CP_UTF8);

    printf("--- 应用程序启动 ---\n");
    printf("目标：通过驱动程序控制虚拟GPIO的电平状态。\n\n");

    // 初始状态检查
    printf(">>> 初始状态检查 <<<\n");
    print_gpio_status();

    // 1. 打开设备
    printf("\n>>> 步骤 1: 打开设备 <<<\n");
    void* device_handle = nuke_driver.open("/dev/nuke0");
    if (!device_handle) {
        fprintf(stderr, "[应用] 错误：无法打开设备！\n");
        return 1;
    }
    printf("[应用] 设备打开成功。\n");

    // 2. 写入 '1' (设置高电平)
    printf("\n>>> 步骤 2: 写入 '1' (设置高电平) <<<\n");
    printf("[应用] 正在调用 driver.write() ...\n");
    nuke_driver.write(device_handle, "1", 1);
    print_gpio_status(); // 验证状态是否变为 0x01

    // 3. 写入 '0' (设置低电平)
    printf("\n>>> 步骤 3: 写入 '0' (设置低电平) <<<\n");
    printf("[应用] 正在调用 driver.write() ...\n");
    nuke_driver.write(device_handle, "0", 1);
    print_gpio_status(); // 验证状态是否变为 0x00

    // 4. 尝试一次无效写入
    printf("\n>>> 步骤 4: 尝试一次无效写入 <<<\n");
    printf("[应用] 正在调用 driver.write() 发送一个无法识别的命令 'ON'...\n");
    nuke_driver.write(device_handle, "ON", 2);
    print_gpio_status(); // 验证状态是否保持不变

    // 5. 关闭设备
    printf("\n>>> 步骤 5: 关闭设备 <<<\n");
    nuke_driver.close(device_handle);
    printf("[应用] 设备已关闭。\n");

    printf("\n--- 应用程序结束 ---\n");

    return 0;
}

/**
 * @brief 打印当前模拟GPIO寄存器的状态，并进行解读。
 */
void print_gpio_status(void) {
    printf("--------------------------------------------------\n");
    printf("[应用] 读取硬件状态: GPIO 寄存器值为 0x%02X ", simulated_gpio_register);
    if (simulated_gpio_register == 0x01) {
        printf("(状态: 高电平/ON)\n");
    } else if (simulated_gpio_register == 0x00) {
        printf("(状态: 低电平/OFF)\n");
    } else {
        printf("(状态: 未知)\n");
    }
    printf("--------------------------------------------------\n");
}