# Sac
Sac is a state-of-the-art lossless audio compression model

Lossless audio compression is a complex problem, because PCM data is highly non-stationary and uses high sample resolution (typically >=16bit). That's why classic context modelling suffers from context dilution problems. Sac employs a simple OLS-NLMS predictor per frame including bias correction. Prediction residuals are encoded using a sophisticated bitplane coder including SSE and various forms of probability estimations. Meta-parameters of the predictor are optimized with [DDS](https://agupubs.onlinelibrary.wiley.com/doi/10.1029/2005WR004723) on by-frame basis. This results in a highly asymmetric codec design.

This program wouldn't exist without the help from the following people (in no particular order):

Matt Mahoney, Dmitry Shkarin, Eugene D. Shelwien, Florin Ghido, Grzegorz Ulacha

## Technical features
* Input: wav file with 1-16 bit sample size, mono/stereo, pcm
* Output: sac file including all input metadata
* Decoded wav file is bit for bit identical to input wav file
* MD5 of raw pcm values

## Technical limitations
Sac uses fp64 for many internal calculations. The change of compiler options or (cpu-)platform might effect the output. Use at your own risk and for testing purposes only.

## Benchmarks
**Sac v0.7.8**

16 files (51.014.742 bytes) parallel on i7-13700H.

**Asymmetric encoding profiles** - bits per sample (bps) is mean bps over all files
|Profile|Size|Enc-time|Dec-time|bps|
|:-|:-|:-|:-|:-|
|--normal|26.812.987|00:00:41|00:00:30|8.441|
|--high|26.708.465|00:04:55|00:00:52|8.407|
|--veryhigh|26.621.497|00:32:04|00:01:04|8.379|
|--best|26.554.323|05:17:19|00:01:05|8.358|

&nbsp;

**Comparison with other lossless audio codecs**
|Program|Parameters|Source|
|:-|:-|:-|
|Sac v0.7.8|--best|open|
|OFR v5.100|--preset max|closed|
|paq8px_v208fix1|-6|open|
|MAC v10.44|--insane|open|

Numbers are bits per sample (bps)
| Name  | Sac | OFR | paq8px | MAC |
|:---|---:|---:|---:|---:|
|ATrain|7.026|7.156|7.353|7,269|
|BeautySlept|7.418|7.790|7.826|8,464|
|chanchan|9.692|9.778|9.723|9,951|
|death2|5.068|5.465|5.215|6,213|
|experiencia|10.866|10.915|10.963|11,005|
|female_speech|4.372|4.498|4.708|5,190|
|FloorEssence|9.081|9.409|9.488|9,537|
|ItCouldBeSweet|8.200|8.310|8.330|8,531|
|Layla|9.490|9.571|9.725|9,783|
|LifeShatters|10.768|10.808|10.868|10,838|
|macabre|8.986|9.026|9.249|9,172|
|male_speech|4.276|4.256|4.509|5,255|
|SinceAlways|10.344|10.409|10.455|10,522|
|thear1|11.379|11.400|11.474|11,451|
|TomsDiner|6.996|7.108|7.057|7,432|
|velvet|9.762|9.990|10.030|10,461|
|*Mean*|**8.358**|8.493|8.561|8,817|

## Compilation

### Zig

`sac` can be easily built for your system using the Zig build system. Building requires Zig version ≥`0.13.0`.

0. Ensure you have Zig & git installed

1. Clone this repo & enter the cloned directory:

    ```bash
    git clone https://github.com/MartinEesmaa/sac/
    cd sac
    ```

2. Build the binary with Zig:

    ```bash
    zig build
    ```
    > Note: If you'd like to specify a different build target from your host OS/architecture, simply supply the target flag. Example: `zig build -Dtarget=x86_64-linux-gnu`

3. Find the build binary in `zig-out/bin`. You can install it like so:

    ```bash
    sudo cp zig-out/bin/sac /usr/local/bin
    ```

Now, you should be all set to use `sac`.

### GCC/G++

To compile SAC audio codec executable program, use for GCC with G++ command by:

```
g++ main.cpp cmdline.cpp ./common/*.cpp ./file/*.cpp ./libsac/*.cpp ./model/*.cpp ./pred/*.cpp -std=c++11 -static -O2 -s -osac
```

For compiling Android binaries, please add additional commands on Clang++ to avoid TLS segment underaligned and merge with G++ same commands:

```
-ffunction-sections -fdata-sections -Wl,--gc-sections
```

Example merged commands:
```
aarch64-linux-android21-clang++ main.cpp cmdline.cpp ./common/*.cpp ./file/*.cpp ./libsac/*.cpp ./model/*.cpp ./pred/*.cpp -std=c++11 -static -ffunction-sections -fdata-sections -Wl,--gc-sections -O2 -s -osac_android-arm64
```

Android Clang++ can be found there in root directory, example:

`android-ndk-r24/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android21-clang++`

To compile Windows 98 binary, you can get [there](https://github.com/fsb4000/gcc-for-Windows98) and compile it:

You need to use the command, cause it doesn't support multiple files with dots, so it has to be each files one at time.

```
g++ main.cpp cmdline.cpp common/md5.cpp common/utils.cpp file/file.cpp file/sac.cpp file/wav.cpp libsac/libsac.cpp libsac/map.cpp libsac/pred.cpp libsac/profile.cpp libsac/vle.cpp model/range.cpp pred/rls.cpp -std=c++11 -static -O2 -s -osac
```

To cross-compile MS-DOS 32-bit executable, you can get latest [build-djgpp](https://github.com/andrewwutw/build-djgpp) and also need to pass it with `-fpermissive` flag. It's available for Windows, macOS and Linux.

You can run MS-DOS 32-bit executable by Microsoft MS-DOS version 5.00 and later, DOSBox or FreeDOS to run for it.
