# 图片资源工作流程

这个文档展示了如何在STM32 Clean Architecture项目中使用图片资源的完整工作流程。

## 🖼️ 准备图片资源

### 1. 图片格式要求
- **支持格式**: PNG, JPEG, GIF, BMP
- **推荐尺寸**: 80×160 (匹配ST7735显示屏)
- **颜色深度**: 24位RGB (工具会自动转换为RGB565)

### 2. 文件命名规范
按用途命名图片文件，工具会根据文件名自动识别用途：

```
assets/images/
├── boot_logo.png          # 开机画面
├── shutdown_logo.png      # 关机画面  
├── meter_af.gif          # VU表动画(12帧)
├── battery_low.png       # 低电量图标
└── background.png        # 背景图片
```

## 🔄 转换流程

### 步骤1: 单个图片转换
```bash
# 转换单个PNG文件
python tools/img2c.py assets/images/boot_logo.png

# 转换GIF动画
python tools/img2c.py assets/images/meter_af.gif

# 指定输出格式
python tools/img2c.py --format RGB565 assets/images/background.png
```

### 步骤2: 批量转换
```bash
# 转换整个images目录
python tools/convert_assets.py

# 同时生成集成代码示例
python tools/convert_assets.py --generate-integration
```

## 📁 生成的文件结构

转换完成后会生成以下文件：

```
assets/
├── images/                    # 原始图片文件
│   ├── boot_logo.png
│   ├── shutdown_logo.png
│   └── meter_af.gif
├── generated/                 # 自动生成的C代码
│   ├── boot_logo.h           # 头文件声明
│   ├── boot_logo.c           # 图片数据数组
│   ├── shutdown_logo.h
│   ├── shutdown_logo.c
│   ├── meter_af.h            # 动画帧数组
│   └── meter_af.c
├── resources.h               # 主资源头文件
├── resources.c               # 资源管理实现
└── ui_integration_example.c  # 使用示例代码
```

## 🔗 项目集成

### 1. 更新Makefile
工具会自动更新Makefile，添加生成的.c文件：

```makefile
C_SOURCES = \
app/main.c \
...
assets/generated/boot_logo.c \
assets/generated/shutdown_logo.c \
assets/generated/meter_af.c
```

### 2. 包含头文件
在你的UI代码中包含资源头文件：

```c
#include "assets/resources.h"
```

### 3. 使用图片数据

#### 静态图片显示
```c
void show_boot_logo(void) {
    // 清屏
    ST7735_FillScreen(ST7735_BLACK);
    
    // 居中显示logo
    uint16_t x = (ST7735_WIDTH - BOOT_LOGO_WIDTH) / 2;
    uint16_t y = (ST7735_HEIGHT - BOOT_LOGO_HEIGHT) / 2;
    
    // 绘制图片
    ST7735_DrawImage(x, y, BOOT_LOGO_WIDTH, BOOT_LOGO_HEIGHT, boot_logo_data);
}
```

#### 动画播放
```c
void animate_vu_meter(void) {
    static uint8_t current_frame = 0;
    static uint32_t last_update = 0;
    
    uint32_t now = HAL_GetTick();
    if (now - last_update >= 50) {  // 20 FPS
        last_update = now;
        
        // 获取当前帧数据
        const uint16_t* frame_data = meter_af_frame_ptrs[current_frame];
        
        // 绘制帧
        ST7735_DrawImage(0, 0, METER_AF_WIDTH, METER_AF_HEIGHT, frame_data);
        
        // 切换到下一帧
        current_frame = (current_frame + 1) % METER_AF_FRAMES;
    }
}
```

## 🛠️ 实际案例

以原始WB32L003项目为例：

### 原始需求
```
- boot_logo.png 160×80、16-bit
- off_logo.png  160×80、16-bit  
- meter_af.gif  12 frames, 20 fps
```

### 转换过程
```bash
# 1. 放置文件
cp boot_logo.png assets/images/
cp off_logo.png assets/images/shutdown_logo.png  # 重命名为标准名称
cp meter_af.gif assets/images/

# 2. 批量转换
python tools/convert_assets.py --generate-integration

# 3. 检查生成结果
ls assets/generated/
# 输出: boot_logo.h boot_logo.c shutdown_logo.h shutdown_logo.c meter_af.h meter_af.c
```

### 集成到UI代码
```c
// 在 ui_management.c 中
#include "assets/resources.h"

void ui_show_startup_sequence(void) {
    const resource_info_t* logo = find_resource("boot_logo");
    if (logo != NULL) {
        const uint16_t* data = (const uint16_t*)get_resource_data(logo, 0);
        ST7735_DrawImage(0, 0, logo->width, logo->height, data);
    }
}

void ui_update_vu_meter(uint8_t left_level, uint8_t right_level) {
    // 先绘制基础背景动画
    animate_vu_meter();
    
    // 叠加实时电平显示
    draw_level_bars(left_level, right_level);
}
```

## 📊 内存优化

### 查看内存使用
```bash
# 编译后查看内存使用
make size

# 查看资源内存占用
python -c "
from tools.convert_assets import AssetManager
manager = AssetManager('.')
print(f'Total resource memory: {manager.calculate_total_resource_memory()} bytes')
"
```

### 优化策略
1. **减少颜色深度**: 使用GRAYSCALE格式
2. **压缩图片**: 使用--compress选项 
3. **减少帧数**: 对动画进行抽帧
4. **按需加载**: 只转换实际需要的图片

## 🎯 最佳实践

### 1. 文件组织
- 按功能分组图片文件
- 使用一致的命名规范
- 保留原始图片文件以便后续修改

### 2. 开发流程
- 先用低分辨率图片测试功能
- 确认功能正常后再使用最终图片
- 定期检查flash内存使用情况

### 3. 版本控制
- .gitignore中排除generated/目录
- 保留原始图片文件在版本控制中
- 在CI/CD中自动运行转换脚本

## 🚨 常见问题

### Q: 转换后文件过大，超出flash限制怎么办？
A: 
1. 减少图片尺寸
2. 使用GRAYSCALE格式
3. 减少动画帧数
4. 启用压缩选项

### Q: 动画播放不流畅怎么办？ 
A:
1. 检查帧率设置(建议20fps)
2. 确保主循环及时调用动画函数
3. 避免在动画更新中进行耗时操作

### Q: 如何添加新的图片资源？
A:
1. 将图片放入assets/images/目录
2. 运行批量转换脚本
3. 更新Makefile包含新的.c文件
4. 在代码中使用新的数组名

---

这个工作流程确保了从图片设计到嵌入式代码的无缝转换，大大提高了开发效率。