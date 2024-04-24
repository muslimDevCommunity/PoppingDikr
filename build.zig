//بسم الله الرحمن الرحيم
//la ilaha illa Allah mohammed rassoul Allah
const std = @import("std");

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.
pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const dikr = b.addExecutable(.{ .name = "dikr", .target = target, .optimize = optimize });
    b.installArtifact(dikr);
    dikr.linkLibC();
    dikr.addCSourceFiles(&.{"src/dikr.c"}, &.{ "-std=c11", "-Wall" });

    const settings = b.addExecutable(.{ .name = "settings", .target = target, .optimize = optimize });
    b.installArtifact(settings);
    settings.linkLibC();
    settings.addCSourceFiles(&.{"src/settings.c"}, &.{ "-std=c11", "-Wall" });

    // const sdl_dep = b.dependency("SDL", .{
    //     .optimize = .ReleaseFast,
    //     .target = target,
    // });
    // dikr.linkLibrary(sdl_dep.artifact("SDL2"));
    // b.installArtifact(dikr);

    if (target.isNativeOs() and target.getOsTag() == .linux) {
        // The SDL package doesn't work for Linux yet, so we rely on system
        // packages for now.
        settings.linkSystemLibrary("SDL2");
        settings.linkLibC();
        dikr.linkSystemLibrary("SDL2");
        dikr.linkLibC();
    } else {
        const sdl_dep = b.dependency("SDL", .{
            .optimize = .ReleaseFast,
            .target = target,
        });
        dikr.linkLibrary(sdl_dep.artifact("SDL2"));
        settings.linkLibrary(sdl_dep.artifact("SDL2"));
    }

    b.installArtifact(dikr);
    b.installArtifact(settings);

    const run_cmd = b.addRunArtifact(settings);
    //run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
