<!-- markdownlint-disable -->

<div align="center">

<img alt="LOGO" src="./resource/icon.png" width="256" height="256" />

# 发声键盘 - DesktopDanmaku

V 0.0.0.0 开发中

<br>
<div>
    <img alt="C++" src="https://img.shields.io/badge/C++-17-%2300599C?logo=cplusplus">
</div>
<div>
    <img alt="platform" src="https://img.shields.io/badge/platform-Windows-blueviolet">
</div>
<div>
    <img alt="license" src="https://img.shields.io/github/license/xiaoditx/DesktopDanmaku">
    <img alt="commit" src="https://img.shields.io/github/commit-activity/m/xiaoditx/DesktopDanmaku">
</div>
<div>
    <img alt="stars" src="https://img.shields.io/github/stars/xiaoditx/DesktopDanmaku?style=social">
    <img alt="GitHub all releases" src="https://img.shields.io/github/downloads/xiaoditx/DesktopDanmaku/total?style=social">
</div>
</div>
<br>
<div>
    <a href="#开发信息">贡献者帮助信息</a>
</div>
<br>

<!-- markdownlint-restore -->

灵感来源于：[用 Visual Basic 写的桌面弹幕程序](https://github.com/wsrj/desk-danmaku-vb)

### （计划的）功能

- 在桌面上显示弹幕
- 支持从json文件导入弹幕内容
- 支持自定义弹幕透明度

### 构建方式

使用下面的命令编译标准软件

```batch
make
```

使用下面的命令编译32位

```batch
make ARCH=32
```

makefile从[keybonk项目](https://github.com/xiaoditx/keybonk)中移植而来，可能会有无用的目标，最好不要使用因为未经测试

### 开发信息

项目文档：[docs](./docs)

软件技术栈

- win32
- C++17

开发环境

- Windows10-x64
- MSYS2（MingGW64）
- VScode
- GNU Make（4.4.1）

## 贡献者

感谢社区的贡献者们（其实现在并没有）

<a href="https://github.com/xiaoditx/DesktopDanmaku/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=xiaoditx/DesktopDanmaku" />
</a>

## 项目统计

<a href="https://www.star-history.com/#xiaoditx/DesktopDanmaku&type=date&legend=top-left">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=xiaoditx/DesktopDanmaku&type=date&theme=dark&legend=top-left" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=xiaoditx/DesktopDanmaku&type=date&legend=top-left" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=xiaoditx/DesktopDanmaku&type=date&legend=top-left" />
 </picture>
</a>

![Alt](https://repobeats.axiom.co/api/embed/c1379970865ed874223601ece505247dc8d6fe9b.svg "Repobeats analytics image")
