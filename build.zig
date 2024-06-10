//بسم الله الرحمن الرحيم
//la ilaha illa Allah Mohammed Rassoul Allah
const std = @import("std");

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const dikr = b.addExecutable(.{
        .name = "dikr",
        .target = target,
        .optimize = optimize,
    });

    dikr.addCSourceFile(.{ .file = std.Build.LazyPath.relative("src/dikr.c") });
    dikr.linkLibC();

    if (target.query.isNativeOs() and target.result.isGnu()) {
        dikr.linkSystemLibrary("sdl2");
        dikr.linkSystemLibrary("sdl2_ttf");
    } else if (target.result.isMinGW()) {
        dikr.addIncludePath(std.Build.LazyPath.relative("submodules/SDL2-2.30.3/x86_64-w64-mingw32/include/SDL2"));
        dikr.addIncludePath(std.Build.LazyPath.relative("submodules/SDL2_ttf-2.22.0/x86_64-w64-mingw32/include/SDL2"));

        dikr.addObjectFile(std.Build.LazyPath.relative("submodules/harfbuzz-win64/libfreetype-6.dll"));
        dikr.addObjectFile(std.Build.LazyPath.relative("submodules/harfbuzz-win64/libharfbuzz-0.dll"));
        dikr.addObjectFile(std.Build.LazyPath.relative("submodules/SDL2-2.30.3/x86_64-w64-mingw32/bin/SDL2.dll"));
        dikr.addObjectFile(std.Build.LazyPath.relative("submodules/SDL2_ttf-2.22.0/x86_64-w64-mingw32/lib/libSDL2_ttf.a"));
    }
    b.installArtifact(dikr);

    const run_cmd = b.addRunArtifact(dikr);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
