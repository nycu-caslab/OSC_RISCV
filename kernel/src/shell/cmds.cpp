#include "shell/cmd.hpp"

const Cmd cmds[] = {
    {
        .name = "help",
        .help = "print this help menu",
        .fp = cmd_help,
    },
    {
        .name = "hello",
        .help = "print Hello World!",
        .fp = cmd_hello,
    },
    {
        .name = "reboot",
        .help = "reboot the device",
        .fp = cmd_reboot,
    },
    {
        .name = "fdt",
        .help = "print device tree",
        .fp = cmd_fdt,
    },
    {
        .name = "ls",
        .help = "list directory contents",
        .fp = cmd_ls,
    },
    {
        .name = "cat",
        .help = "concatenate and print files",
        .fp = cmd_cat,
    },
    {
        .name = "mm",
        .help = "memory allocation",
        .fp = cmd_mm,
    },
    {
        .name = "screen_color",
        .help = "write color to frame buffer",
        .fp = cmd_screen_color,
    },
    {
        .name = "screen_cat",
        .help = "write file to frame buffer",
        .fp = cmd_screen_cat,
    },
};
const int ncmd = sizeof(cmds) / sizeof(cmds[0]);
