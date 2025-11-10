# ADC DMA Configuration with HRTIM Hardware Triggering

## 概述 (Overview)

本文档描述了 ADC 从软件轮询模式迁移到 HRTIM 硬件触发 + DMA 自动传输模式的配置更改。

This document describes the configuration changes to migrate ADC from software polling mode to HRTIM hardware-triggered mode with automatic DMA transfer.

## 修改前的问题 (Problems Before Modification)

### 1. ADC 触发方式不匹配
- **问题**: ADC 配置为 `ADC_SOFTWARE_START`，但 HRTIM 已配置 ADC 触发器
- **后果**: ADC 采样由软件控制在 100Hz (每 10ms)，而不是硬件同步在 50kHz PWM 周期
- **影响**: 无法实现 FOC 电流环所需的高频同步采样

### 2. 缺少 DMA 配置
- **问题**: ADC1 没有配置 DMA 传输，只有 ADC2 有 DMA
- **后果**: ADC1 数据需要软件轮询读取，增加 CPU 负载
- **影响**: 实时性差，无法在中断中快速处理电流数据

### 3. 主循环阻塞式采样
- **问题**: `main.cpp` 中使用 `ADC1_Channel_conversion()` 阻塞式调用
- **后果**: 每次 ADC 采样都会阻塞程序执行
- **影响**: 不适合实时电机控制应用

## 修改内容 (Modifications)

### 1. ADC1 配置更改 (`bsp_adc.cpp`)

#### ADC1 初始化结构体
```cpp
// 修改前 (Before):
hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
hadc1.Init.NbrOfConversion = 1;
hadc1.Init.ContinuousConvMode = ENABLE;
hadc1.Init.DMAContinuousRequests = DISABLE;
hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;

// 修改后 (After):
hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_HRTIM_TRG1;  // HRTIM 触发
hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;  // 上升沿触发
hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;  // 使能扫描模式
hadc1.Init.NbrOfConversion = 3;  // 3 个通道 (三相电流)
hadc1.Init.ContinuousConvMode = DISABLE;  // HRTIM 触发，非连续模式
hadc1.Init.DMAContinuousRequests = ENABLE;  // 使能 DMA 连续请求
hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;  // 覆盖旧数据
```

#### ADC1 通道配置
```cpp
// 修改前: 单通道 CH4
// 修改后: 三通道 CH1/CH2/CH3 用于三相电流采样
sConfig.Channel = ADC_CHANNEL_1;  // Phase A current
sConfig.Rank = ADC_REGULAR_RANK_1;
sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;  // 增加采样时间以提高稳定性

// CH2: Phase B current (Rank 2)
// CH3: Phase C current (Rank 3)
```

### 2. ADC2 配置更改 (`bsp_adc.cpp`)

```cpp
// 修改前 (Before):
hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
hadc2.Init.ContinuousConvMode = ENABLE;
hadc2.Init.DMAContinuousRequests = DISABLE;

// 修改后 (After):
hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIG_HRTIM_TRG2;  // HRTIM 触发 2
hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
hadc2.Init.ContinuousConvMode = DISABLE;  // HRTIM 触发，非连续模式
hadc2.Init.DMAContinuousRequests = ENABLE;  // 使能 DMA 连续请求
```

### 3. DMA 配置 (`bsp_adc.cpp`)

#### 新增 ADC1 DMA 配置
```cpp
// 在 HAL_ADC_MspInit() 中为 ADC1 添加 DMA 配置
hdma_adc1.Instance = DMA1_Channel1;
hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
hdma_adc1.Init.Mode = DMA_CIRCULAR;  // 循环模式用于连续采样
hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;  // 高优先级用于 FOC 电流环

__HAL_LINKDMA(adcHandle, DMA_Handle, hdma_adc1);
```

#### 更新 ADC2 DMA 配置
```cpp
// 修改前: DMA_NORMAL 模式，DMA_PRIORITY_LOW
// 修改后: DMA_CIRCULAR 模式，DMA_PRIORITY_MEDIUM
hdma_adc2.Init.Mode = DMA_CIRCULAR;  // 循环模式
hdma_adc2.Init.Priority = DMA_PRIORITY_MEDIUM;  // 中等优先级
```

### 4. DMA 缓冲区 (`bsp_adc.cpp`)

```cpp
// 新增全局 DMA 缓冲区
uint16_t adc1_dma_buffer[3] = {0};  // 三相电流 (Phase A, B, C)
uint16_t adc2_dma_buffer[2] = {0};  // 其他信号 (例如 Vbus, 温度)

// 在头文件中导出
extern uint16_t adc1_dma_buffer[3];
extern uint16_t adc2_dma_buffer[2];
```

### 5. 主程序更改 (`main.cpp`)

```cpp
int main()
{
    bsp_init();
    
    // 启动 ADC DMA (由 HRTIM 硬件触发，~50kHz 采样)
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc1_dma_buffer, 3);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc2_dma_buffer, 2);
    
    // 使能 HRTIM 定时器输出 (开始 PWM 生成和 ADC 触发)
    hrtim_timerA_output_enable();
    hrtim_timerB_output_enable();
    hrtim_timerE_output_enable();

    while (1) {
        // ... 其他代码 ...
        
        // 删除旧的软件轮询代码:
        // adc_conversion = ADC1_Channel_conversion();
        // ADC2_Channel_Conversion(adc2_conversion_sequence);
        
        // ADC 数据现在自动保存在 adc1_dma_buffer[] 和 adc2_dma_buffer[] 中
        // FOC 电流环处理应在 HAL_ADC_ConvCpltCallback() 中实现
    }
}
```

### 6. ADC 转换完成回调 (`bsp_adc.cpp`)

```cpp
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) 
{
    if (hadc->Instance == ADC1) {
        // ADC1 转换完成 - 三相电流数据在 adc1_dma_buffer[] 中
        // TODO: 在此处实现 FOC 电流环控制
        
        // 示例: 读取相电流
        uint16_t ia_raw = adc1_dma_buffer[0];  // A 相
        uint16_t ib_raw = adc1_dma_buffer[1];  // B 相
        uint16_t ic_raw = adc1_dma_buffer[2];  // C 相
        
        // TODO: Clarke 变换: abc → αβ
        // TODO: Park 变换: αβ → dq (使用转子角度)
        // TODO: Id/Iq PI 调节器
        // TODO: 反 Park 变换: dq → αβ
        // TODO: 反 Clarke 变换或 SVPWM
        // TODO: 更新 HRTIM 比较寄存器以控制 PWM 占空比
    }
    else if (hadc->Instance == ADC2) {
        // ADC2 转换完成 - Vbus/温度数据在 adc2_dma_buffer[] 中
        // TODO: 处理辅助信号（母线电压监控、过流保护等）
    }
}
```

## 工作原理 (How It Works)

### 时序图
```
HRTIM Timer A (50kHz PWM, 中央对齐模式)
    ↓ Counter reaches period (crest point)
    ↓
HRTIM ADC Trigger 1 (TIMERA_PERIOD event)
    ↓
ADC1 开始转换 3 个通道 (CH1, CH2, CH3)
    ↓
ADC1 转换完成 → 触发 DMA1_Channel1
    ↓
DMA 自动将数据传输到 adc1_dma_buffer[3]
    ↓
DMA 完成 → 触发 ADC1_2_IRQn 中断
    ↓
调用 HAL_ADC_ConvCpltCallback(&hadc1)
    ↓
处理电流数据，执行 FOC 算法，更新 PWM 占空比
```

### 采样时刻
- **PWM 模式**: 中央对齐 (UPDOWN 模式)
- **触发事件**: TIMERA_PERIOD (计数器到达峰值时刻)
- **采样频率**: ~50kHz (与 PWM 频率相同)
- **采样位置**: PWM 波形的波峰 (crest)，此时电流纹波最小

### 为什么在波峰采样？
对于中央对齐 PWM (上下计数模式):
- 波峰时刻电流纹波达到稳定状态
- 避开开关瞬态，减少测量噪声
- FOC 算法需要稳定的电流测量值

## 配置参数说明

### ADC 采样时间
- **设置**: `ADC_SAMPLETIME_47CYCLES_5` (47.5 个 ADC 时钟周期)
- **ADC 时钟**: 170MHz / 4 = 42.5MHz (PCLK_DIV4)
- **采样时间**: 47.5 / 42.5MHz ≈ 1.12μs
- **单通道转换时间**: (47.5 + 12.5) / 42.5MHz ≈ 1.41μs (12.5 为 12-bit 转换周期)
- **3 通道总时间**: ≈ 4.23μs (远小于 50kHz PWM 周期的 20μs)

### DMA 优先级
- **ADC1 DMA**: `DMA_PRIORITY_HIGH` - FOC 电流环需要高优先级
- **ADC2 DMA**: `DMA_PRIORITY_MEDIUM` - 辅助信号优先级较低

### DMA 模式
- **循环模式** (`DMA_CIRCULAR`): DMA 自动重新加载，无需软件干预
- **连续请求** (`DMAContinuousRequests = ENABLE`): 每次 ADC 转换完成自动触发 DMA

## 电流传感器配置 (示例)

假设使用运放 + 采样电阻的电流传感器:
```cpp
// 硬件参数
#define VREF 1.65f          // 参考电压 (运放偏置)
#define SENSITIVITY 0.066f   // 灵敏度 66mV/A (采样电阻 + 放大系数)
#define ADC_VMAX 3.3f       // ADC 最大电压
#define ADC_RESOLUTION 4096 // 12-bit ADC

// ADC 原始值转换为电流 (A)
float adc_to_current(uint16_t adc_raw) {
    float voltage = adc_raw * ADC_VMAX / ADC_RESOLUTION;
    float current = (voltage - VREF) / SENSITIVITY;
    return current;
}

// 在回调中使用
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    if (hadc->Instance == ADC1) {
        float ia = adc_to_current(adc1_dma_buffer[0]);
        float ib = adc_to_current(adc1_dma_buffer[1]);
        float ic = adc_to_current(adc1_dma_buffer[2]);
        
        // 继续 FOC 算法...
    }
}
```

## 性能对比

| 特性 | 修改前 (软件轮询) | 修改后 (HRTIM + DMA) |
|------|------------------|---------------------|
| 采样频率 | 100Hz (每 10ms) | 50kHz (每 20μs) |
| CPU 占用 | 高 (需要轮询) | 低 (DMA 自动传输) |
| 实时性 | 差 (延迟 10ms) | 优秀 (延迟 <5μs) |
| PWM 同步 | 无 | 完美同步 (硬件触发) |
| FOC 适用性 | 不适用 | 适用 (满足 20kHz 电流环要求) |

## 下一步开发 (Next Steps)

1. **实现 FOC 算法**
   - Clarke 变换 (abc → αβ)
   - Park 变换 (αβ → dq)
   - PI 电流调节器
   - SVPWM 调制

2. **添加保护功能**
   - 过流检测 (ADC 值超限检查)
   - 过压/欠压检测 (监控 Vbus)
   - 温度保护

3. **调试和验证**
   - 使用示波器验证 ADC 采样时刻
   - 检查 DMA 缓冲区数据正确性
   - 测量 CPU 负载和中断延迟

4. **性能优化**
   - 使用 CORDIC 加速三角函数计算
   - 优化 PI 控制器参数
   - 实现前馈补偿

## 参考文档

- `doc/hrtim.md` - HRTIM 配置详细说明
- `doc/adc.md` - ADC 配置详细说明
- STM32G474 Reference Manual (RM0440)
- AN5346: HRTIM Cookbook for motor control applications
