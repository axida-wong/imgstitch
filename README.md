# imgstitch

---

将 `r*c` 个 `h*w` 的 TIFF 子图按网格拼接成一张大的 TIFF 图像。本项目的**核心代码没有 AI 接入**，完全使用纯 C 语言和 libtiff 库手工编写。

---

## 依赖

- C 编译器（如 `gcc`，支持 C23）
- libtiff 开发库
  - Debian/Ubuntu：`sudo apt install libtiff-dev`
  - RHEL/Fedora：`sudo dnf install libtiff-devel`

## 编译

在项目根目录执行：

```bash
make
```

编译产物为 `bin/imgstitch`。清理编译产物：

```bash
make clean
```

## 用法

```bash
./bin/imgstitch -r <行数> -c <列数> -h <子图高度> -w <子图宽度> -d <输入目录> [-o <输出目录>]
```

### 参数说明

| 参数 | 短选项 | 含义                                       |
|------|--------|--------------------------------------------|
| rows    | `-r` | 拼接网格的行数                            |
| cols    | `-c` | 拼接网格的列数                            |
| height  | `-h` | 单个子图的高度（像素）                    |
| width   | `-w` | 单个子图的宽度（像素）                    |
| dir     | `-d` | 存放输入子图的目录路径                    |
| out     | `-o` | 输出目录（可选，默认取当前目录 `.`）      |

### 输入文件命名约定

输入目录下需要存在 `rows*cols` 个 TIFF 子图，按 **1 到 rows*cols 顺序编号**，文件名补零到总文件数的位数。例如总共 16 张则为：

```
01.tif  02.tif  ...  16.tif
```

编号按行优先排列，即前 `cols` 个文件构成网格的第一行，依此类推。

### 运行示例

```bash
# 将 4x4 共 16 张 256x256 的子图拼成一张 1024x1024 的大图
./bin/imgstitch -r 4 -c 4 -h 256 -w 256 -d ./test_images
```

运行后会在输出目录生成拼接结果 `result.tif`。

本项目自带 `test.sh` 脚本，可直接运行演示：

```bash
./test.sh
```

## 核心代码结构

```
src/
├── argparse.c   命令行参数解析（getopt_long），填充全局 options
├── argparse.h   参数结构体与解析函数声明
├── imgstitch.c  核心拼接逻辑，负责打开子图、逐行读取并写入大图
├── imgstitch.h  Imgs 结构体与拼接相关函数声明
└── main.c       程序入口，串联「解析 → 打开 → 拼接 → 关闭」流程
```

### 拼接原理

1. 打开 `rows*cols` 个子图，按行优先线性排列。
2. 以第一个子图为模板读取像素属性（每像素采样数、位深、光度模型等）。
3. 配置输出大图的宽高 = `cols*width × rows*height`。
4. 逐行遍历：按行把同一网格行内各子图的对应扫描线拼成一条完整扫描线，再写入大图。
