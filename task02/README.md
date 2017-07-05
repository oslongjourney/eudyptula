# Brief

# A very brief 'how to compile'

> Few steps to compile:

1. Take a working .config file. Two main options:
  * From proc: zcat /proc/config.gz > .config
  * From boot dir: cp /boot/config-$(uname -a) .config
2. "Default all questions based on the content of .config": make olddefconfig
3. If required, make specific configurations with: make nconfig
4. Compile kernel: make -j<number_of_cpu_cores>
5. Compile modules: make modules_install

> Fast compile approach:

1. Compile only for your specific architecture:
```
make ARCH=x86_64 -j<cpu_cores>
```
2. Create a .config file based on your current enabled modules:
```
make localmodconfig
make olddefconfig
```
Warning: Enable all devices that you usually use before execute localmodconfig,
otherwise some of your devices may not work because of devices missing

# Some make commands

> Small list of useful make commands:

1. make localmodconfig
2. make olddefconfig
3. make -j<cpu_cores>
4. make modules
5. sudo make install
6. sudo make modules_install
7. sudo make ARCH=x86_64 INSTALL_HDR_PATH=/usr/ headers_install
8. make clean

## Configuration menus

> There is five different option to configure the Kernel interactively:

1. make menuconfig: Open a ncurse menu to configure the options
2. make nconfig: Similar to menuconfig
3. make xconfig: A graphical interface using Qt
4. make gconfig: A graphical interface using gtk+
5. make config: A iterative option (one option by one)

> What I prefer: nconfig followed by menucofig.

> Extra note: If you type '?' on one of the options inside the menu, you can get
a very useful information about the option

# Differences between Linux final results

## What is vmlinu[z|x] and bzImage?

* vmlinux is statically linked executable, usually, it is used for debugging and
  symbol table generation.
* "vm" came from "virtual memory", and "z" means compressed (e.x.: gzipped).
* bzImage (big zImage): It was created to overcome the problem of the limited
  space available for compressed kernel. Basically, it splits the kernel over
  discontinuous memory regions.

# Initramfs and cpio

# qemu

qemu-system-x86_64  -enable-kvm -kernel arch/x86_64/boot/bzImage -initrd initramfs_hello.cpio.gz -hda disk.img

# Boot on local machine

# Reference

* Initramfs, practical stuffs: http://nairobi-embedded.org/initramfs_tutorial.html
* Initramfs general: https://en.wikipedia.org/wiki/Initramfs
* Great explanation about initramfs: https://landley.net/writing/rootfs-intro.html
* A very cool boot on personal kernel: https://landley.net/writing/rootfs-howto.html
* vmlinuz definition: http://www.linfo.org/vmlinuz.html
* vmlinuz an bzImage: https://en.wikipedia.org/wiki/Vmlinux
* Stackexchange about initram: https://unix.stackexchange.com/questions/48302/running-bzimage-in-qemu-unable-to-mount-root-fs-on-unknown-block0-0
* Kernel readme: https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/README?id=refs/tags/v4.3.3
* Speedup compile: http://www.h-online.com/open/features/Good-and-quick-kernel-configuration-creation-1403046.html
* Stackoverflow with some tips about speedup kernel complie: https://stackoverflow.com/questions/23279178/how-to-speed-up-linux-kernel-compilation
* SYSTEMMD-BOOT: https://wiki.archlinux.org/index.php/systemd-boot#Adding_boot_entries
* cpio tutorial: https://www.gnu.org/software/cpio/manual/html_mono/cpio.html
* Busybox, build: https://busybox.net/FAQ.html#build_system
