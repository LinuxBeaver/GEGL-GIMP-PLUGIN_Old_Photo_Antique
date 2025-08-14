# GEGL---Old-Photo-Effect---Make-your-photos-have-early-20th-century-aesthetics

GIMP Plugin that makes photos look old and retro. Download below 

https://github.com/LinuxBeaver/GEGL-GIMP-PLUGIN_Old_Photo_Antique/releases/

![image preview](preview2.png )

Look in filters>decor to find it.

## OS specific location to put GEGL Filter binaries 

Windows
 C:\\Users\<YOUR NAME>\AppData\Local\gegl-0.4\plug-ins
 
 Linux 
`~/.local/share/gegl-0.4/plug-ins`
 
 Linux (Flatpak)
`~/.var/app/org.gimp.GIMP/data/gegl-0.4/plug-ins`

## Compiling and Installing

### Linux

To compile and install you will need the GEGL header files (`libgegl-dev` on
Debian based distributions or `gegl` on Arch Linux) and meson (`meson` on
most distributions).

```bash
meson setup --buildtype=release build
ninja -C build

```

If you have an older version of gegl you may need to copy to `~/.local/share/gegl-0.3/plug-ins`
instead (on Ubuntu 18.04 for example).



### Windows

The easiest way to compile this project on Windows is by using msys2.  Download
and install it from here: https://www.msys2.org/

Open a msys2 terminal with `C:\msys64\mingw64.exe`.  Run the following to
install required build dependencies:

```bash
pacman --noconfirm -S base-devel mingw-w64-x86_64-toolchain mingw-w64-x86_64-meson mingw-w64-x86_64-gegl
```

Then build the same way you would on Linux:

```bash
meson setup --buildtype=release build
ninja -C build
```

## Sister plugin

This plugin has a sister plugin that debuted three years after its release (2022 - 2025) named Antique 2

You can check it out here 

https://github.com/LinuxBeaver/GEGL-GIMP-PLUGIN_Old_Photo_Antique2/
