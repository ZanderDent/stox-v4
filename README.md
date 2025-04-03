# stox-v4
A Qt6-based stock trading application with machine learning capabilities using mlpack.

## Prerequisites

Before installing, ensure you have the following dependencies installed:

### macOS (using Homebrew)
```bash
# Install Qt6
brew install qt@6

# Install mlpack and its dependencies
brew install mlpack
brew install armadillo
brew install openblas
brew install lapack

# Install other dependencies
brew install boost
brew install curl
brew install nlohmann-json
```

### Windows
1. Install Visual Studio 2022 Community Edition with:
   - Desktop development with C++
   - Windows 10/11 SDK

2. Install Qt6:
   - Download Qt Online Installer from [Qt Downloads](https://www.qt.io/download)
   - Install Qt 6.5.0 or later with MSVC 2022 64-bit
   - Add Qt's bin directory to PATH (e.g., `C:\Qt\6.5.0\msvc2022_64\bin`)

3. Install vcpkg:
```batch
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

4. Install dependencies via vcpkg:
```batch
vcpkg install mlpack:x64-windows
vcpkg install armadillo:x64-windows
vcpkg install openblas:x64-windows
vcpkg install boost:x64-windows
vcpkg install curl:x64-windows
vcpkg install nlohmann-json:x64-windows
```

## Installation

### macOS
1. Clone the repository:
```bash
git clone https://github.com/yourusername/stox-v4.git
cd stox-v4
```

2. Set up the environment variables:
```bash
export PKG_CONFIG_PATH="/opt/homebrew/opt/qt/lib/pkgconfig:/opt/homebrew/lib/pkgconfig"
export LDFLAGS="-L/opt/homebrew/opt/lapack/lib -L/opt/homebrew/opt/openblas/lib"
export CPPFLAGS="-I/opt/homebrew/opt/lapack/include -I/opt/homebrew/opt/openblas/include"
```

3. Build the application:
```bash
# Clean any existing build artifacts
make clean

# Build the application
make
```

### Windows
1. Clone the repository:
```batch
git clone https://github.com/yourusername/stox-v4.git
cd stox-v4
```

2. Generate Visual Studio solution:
```batch
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake -DCMAKE_PREFIX_PATH=C:/Qt/6.5.0/msvc2022_64
```

3. Build the project:
   - Open `stox-v4.sln` in Visual Studio
   - Select Release configuration
   - Build Solution (F7)

## Running the Application

### macOS
1. Ensure you have an internet connection (required for fetching stock data)

2. Run the application:
```bash
./stockTraderApp
```

### Windows
```batch
Release\stockTraderApp.exe
```

Or run directly from Visual Studio with F5.

## Troubleshooting

### macOS Issues
1. Qt-related issues:
```bash
# Relink Qt6
brew unlink qt
brew link qt --force
```

2. Library linking issues:
```bash
# Verify library installations
brew list qt
brew list mlpack
brew list armadillo
```

### Windows Issues
1. Qt not found:
   - Verify Qt installation path in CMAKE_PREFIX_PATH
   - Ensure Qt bin directory is in PATH

2. vcpkg packages not found:
```batch
vcpkg list
vcpkg upgrade
vcpkg remove [package] --recurse
vcpkg install [package]:x64-windows
```

3. Build errors:
   - Clean solution and rebuild
   - Delete build directory and regenerate with cmake
   - Verify all dependencies are installed with vcpkg

## API Key

The application uses Alpha Vantage API for stock data. The default API key is included, but you may want to replace it with your own in `data_fetcher.cpp`:

```cpp
const std::string apiKey = "YOUR_API_KEY_HERE";
```

Get your API key from [Alpha Vantage](https://www.alphavantage.co/support/#api-key)

## License

[Add your license information here]

## Contributing

[Add contribution guidelines if applicable]
