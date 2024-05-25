# NyaFb. Linux framebuffer library

## Setup
Dependency:
```
sudo apt install libpng-dev
```

## Setup on raspberry pi 4
Add ```fbcon=map:2``` into /boot/firmware/cmdline.txt.

## Run
```
cmake /path/to/nyafb
make
./source/apps/fbtest_1 /dev/fb0
```

