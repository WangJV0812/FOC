# OpenOCD 入门指南

- [OpenOCD 入门指南](#openocd-入门指南)
  - [参考](#参考)

在使用 MDK 等 IDE 时，从编译到下载、调试的过程似乎是水到渠成、自然而然的。但是仔细想想，这个过程似乎没有那么自然。一个显而易见的问题是，MDK 是如何连接到调试器，又如何把代码写入到芯片中的。

这个过程远远没有我们所想象的那么自然。事实上一个较为现代的 IDE 大致可以分为 这么几个过程：

1. 代码编码：IDE使用的是QScitinal这样的编辑器库
2. 代码智能提示：后台调用clang软件
3. 代码语法错误动态检测：后台调用gcc或者clang都可以
4. 工程源码组织：IDE使用的是makefile，后台调用make软件
5. 编译：IDE后台调用gcc编译器
6. 连接硬件调试器：IDE通过后台软件OpenOCD连接调试器和板子
7. 下载：IDE给OpenOCD发命令
8. 调试：IDE后台调用gdb，给gdb发命令，gdb再给OpenOCD发命令，OpenOCD再指挥调试器，控制板子。实现单步执行，查看变量值，寄存器等。

但是当我们用 vscode 时，没有人给我们组织这些工具，就需要我们来自己组织了。我们重点讨论一下 OpenOCD 是什么，怎么工作的。

## 参考

[OpenOCD User's Guide](https://openocd.org/doc-release/html/index.html)
