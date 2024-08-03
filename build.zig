const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});

    // Create the executable
    const bin = b.addExecutable(.{
        .name = "sac",
        .target = target,
        .optimize = .ReleaseFast,
    });

    // List include directories
    const include_dirs = [_][]const u8{
        "src/common/",
        "src/file/",
        "src/libsac/",
        "src/model/",
        "src/opt/",
        "src/pred/",
    };

    const cpp_srcs = &[_][]const u8{
        "src/main.cpp",
        "src/cmdline.cpp",
        "src/common/md5.cpp",
        "src/common/utils.cpp",
        "src/file/file.cpp",
        "src/file/sac.cpp",
        "src/file/wav.cpp",
        "src/libsac/libsac.cpp",
        "src/libsac/map.cpp",
        "src/libsac/pred.cpp",
        "src/libsac/profile.cpp",
        "src/libsac/vle.cpp",
        "src/model/range.cpp",
        "src/pred/rls.cpp",
    };

    for (include_dirs) |dir| {
        bin.addIncludePath(b.path(dir));
    }

    // Add C++ source files
    bin.addCSourceFiles(.{
        .files = cpp_srcs,
        .flags = &.{
            "-std=c++11",
            "-static",
            "-O3",
        },
    });

    // Link libc
    bin.linkLibCpp();
    b.installArtifact(bin);
}
