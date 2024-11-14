# Environment Setup

## Compiler Environment

### Windows

#### WSL

For development on Windows it is expected to use WSL Ubuntu (or equivalent distro) as this project is designed for a Raspberry Pi 4. To start, open a Windows Powershell terminal and enter the following commands.

```bash
wsl --install Ubuntu
```

After installing WSL and entering your user info, restart the terminal and open a new terminal in Ubuntu. Now it is time to install the basic tools that will be needed for development. Enter the following commands.

```bash
sudo apt update && sudo apt upgrade
sudo apt install build-essential gdb
sudo apt install clang
sudo apt install cmake
```

WSL should now be ready to go.

#### MacOS

Don't have one to test setup, so good luck.

#### Linux

In a terminal, enter the following commands (may be different depending on distro).

```bash
sudo apt update && sudo apt upgrade
sudo apt install build-essential gdb
sudo apt install clang
sudo apt install cmake
```

## Editor

The editor used for this project is [Visual Studio Code](https://code.visualstudio.com/). Install the editor for your platform for later use.

Install the following extensions in Visual Studio Code.

| Extension                                                                                                          | Required |
| ------------------------------------------------------------------------------------------------------------------ | -------- |
| [WSL (Windows)](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-wsl)                   | Yes      |
| [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)                                    | Yes      |
| [CMake Language Support](https://marketplace.visualstudio.com/items?itemName=josetr.cmake-language-support-vscode) | Yes      |
| [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)                           | Yes      |
