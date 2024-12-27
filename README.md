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
**Sac v0.7.11**

16 files (51.014.742 bytes) parallel on i7-13700H.

**Asymmetric encoding profiles** - bits per sample (bps) is mean bps over all files
|Profile||Size|Enc-time|Dec-time|bps|
|:-|-:|:-|:-|:-|:-|
|FLAC -8|100.0%|29.793.060|00:00:00|00:00:00|9.385|
|--normal|89.6%|26.695.173|00:00:33|00:00:29|8.402|
|--high|89.2%|26.586.867|00:04:55|00:01:05|8.366|
|--veryhigh|89.0%|26.516.301|00:31:23|00:01:12|8.343|
|--best|88.8%|26.455.919|05:00:19|00:01:00|8.324|

&nbsp;

**Comparison with other lossless audio codecs**
|Program|Parameters|Source|
|:-|:-|:-|
|Sac v0.7.11|--best|open|
|OFR v5.100|--preset max|closed|
|paq8px_v208fix1|-6|open|
|MAC v10.44|-c5000|open|
|FLAC v1.4.3|-8|open|

Numbers are bits per sample (bps)
| Name  | Sac | OFR | paq8px | MAC | FLAC |
|:---|---:|---:|---:|---:|---:|
|ATrain|7.005|7.156|7.353|7.269|7.962|
|BeautySlept|7.137|7.790|7.826|8.464|10.134|
|chanchan|9.683|9.778|9.723|9.951|10.206|
|death2|5.043|5.465|5.215|6.213|6.092|
|experiencia|10.861|10.915|10.963|11.005|11.428|
|female_speech|4.365|4.498|4.708|5.190|5.364|
|FloorEssence|9.059|9.409|9.488|9.537|10.201|
|ItCouldBeSweet|8.185|8.310|8.330|8.531|9.064|
|Layla|9.472|9.571|9.725|9.783|10.415|
|LifeShatters|10.760|10.808|10.868|10.838|11.194|
|macabre|8.977|9.026|9.249|9.172|10.043|
|male_speech|4.237|4.256|4.509|5.255|5.674|
|SinceAlways|10.333|10.409|10.455|10.522|11.254|
|thear1|11.368|11.400|11.474|11.451|11.783|
|TomsDiner|6.993|7.108|7.057|7.432|8.572|
|velvet|9.700|9.990|10.030|10.461|10.770|
|*Mean*|**8.324**|8.493|8.561|8.817|9.385|
|||||||
|Enc-time|05:00:19|00:00:09|00:05:37|00:00:01|00:00:00|
|Dec-time|00:01:00|00:00:02|00:05:40|00:00:01|00:00:00|

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
