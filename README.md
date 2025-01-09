# Baja-2025

## Install Instructions

### [Environment Setup](./SETUP.md)

### GLFW Requirements

On Linux and WSL2, extra dependencies are required to build, which can be installed with the following command:

```bash
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
```

### FreeType

To install the [FreeType](https://freetype.org/) library on Linux and WSL2, use the following command:

```bash
sudo apt install libfreetype-dev
```

### OpenCV

To install [OpenCV](https://opencv.org/) for Ubuntu, use the following command and it should be ready to go. This is something that will make working on Windows more difficult as enabling camera access for WSL is difficult.

```bash
sudo apt install libopencv-dev
```
