# mechrevo26

## 编译依赖
- CMake
- Ninja
- arm-none-eabi-gcc
- openocd
- STM32CubeMX（可选）

## 构建步骤
1. 克隆仓库
   ```bash
   git clone https://github.com/zouyidagh/mechrevo26.git
   ```
   ```

2. 进入项目目录
   ```bash
   cd mechrevo26
   ```

3. 配置CMake
   ```bash
   cmake --preset Debug
   ```

3. 构建
   ```bash
   cmake --build --preset Debug
   ```

5 配置烧录文件: 修改`flash.cfg`烧录配置文件内容，对应你使用的烧录器

4. 烧录固件
   ```bash
   openocd -f flash.cfg
   ```
