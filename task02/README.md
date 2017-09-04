# Task 02 description

----

Now that you have written your first kernel module, it's time to take
off the training wheels and move on to building a custom kernel.  No
more distro kernels for you, for this task you must run your own kernel.
And use git!  Exciting isn't it!  No, oh, ok...

The tasks for this round is:
  - download Linus's latest git tree from git.kernel.org (you have to
    figure out which one is his, it's not that hard, just remember what
    his last name is and you should be fine.)
  - build it, install it, and boot it.  You can use whatever kernel
    configuration options you wish to use, but you must enable
    CONFIG_LOCALVERSION_AUTO=y.
  - show proof of booting this kernel.  Bonus points for you if you do
    it on a "real" machine, and not a virtual machine (virtual machines
    are acceptable, but come on, real kernel developers don't mess
    around with virtual machines, they are too slow.  Oh yeah, we aren't
    real kernel developers just yet.  Well, I'm not anyway, I'm just a
    script...)  Again, proof of running this kernel is up to you, I'm
    sure you can do well.

Hint, you should look into the 'make localmodconfig' option, and base
your kernel configuration on a working distro kernel configuration.
Don't sit there and answer all 1625 different kernel configuration
options by hand, even I, a foolish script, know better than to do that!

After doing this, don't throw away that kernel and git tree and
configuration file.  You'll be using it for later tasks, a working
kernel configuration file is a precious thing, all kernel developers
have one they have grown and tended to over the years.  This is the
start of a long journey with yours, don't discard it like was a broken
umbrella, it deserves better than that.

----

# My full answer

## How to compile the kernel, a brief history

0. Git clone kernel from Linus branch
1. I suppose that you are compiling from an OS based on Linux, hence, you just
  need take your current `.config` file before start. There is two ways to do
  it:
  * From proc: `zcat /proc/config.gz > .config` (sometimes not available)
  * From boot directory: `cp /boot/config-$(uname -a) .config`
2. "Default all questions based on the content of .config": `make olddefconfig`
3. If required, make specific configurations with: `make nconfig`
4. Compile kernel: `make -j<number_of_cpu_cores>`
5. Compile modules: `make modules`
6. Install modules: `sudo make modules_install`
7. Install kernel: `make install`
8. Install headers: `sudo make headers_install ARCH=x86_64 INSTALL_HDR_PATH=/usr/`

## Compile only the required code for your machine

Linux kernel provided many useful targets for compilation, one of them is
`localmodconfig`. This option use `lsmod` command to update the current
`config` file based on the loaded modules. This option can considerably reduce
the compilation time since you just compile what is required by your system.
However, it is important to highlight that any module disabled at the moment of
this command will be not included. So, try to plug in any device that you want
to use.

```
make localmodconfig
```

## Create a configuration file focused on guest machine

It is common to use Qemu to emulate a custom version of the kernel. Again,
Linux Makefile already provide some mechanisms to enable some modules required
by virtualisation. If you will run the custom kernel, it is a good idea to
execute:

```
make kvmconfig
```

## Configure your new kernel using menu

Linux provide mechanisms to interactively configure some kernel options. The
following command has the same effect and are list in order of my preferences:

1. `make nconfig`
2. `make menuconfig`
3. `make xconfig`
4. `make gconfig`
5. `make config`

Keep in mind that you can use "?" inside de menuconfig to see a description of
the functionality and you can search inside it too.

# How to run the customized kernel: a brief steps

Suppose that you have a Qemu installed, you can follow the steps below to create
a new disk image and install any distro that you like:

1. Create an qemu image:

```
qemu-img create -f qcow image_file 4G
```

 * -f <format>: You specify here the format, I recommend qcow or raw
 * image_file: It is the image name, you can add any name that you want
 * The last parameter is the image size, chose a good size for you
 * Finally, if you want to check the details about the image after the command,
   just use:

```
qemu-img info image_file
```

2. Install an OS based on an iso file

```
qemu-system-x86_64 -cdrom ~/path/to_iso.iso -boot order=d -drive file=the_image_disk_previously_created,format=qcow -m 2G
```

* Just read the command above and change the parameters as indicated
* It is important to highlight that `-m` specify the memory size, and the
  default is 256Mb which is not enough for modern OS installations

3. You can start your new image with:

```
qemu-system-x86_64 -enable-kvm -net nic -net user,hostfwd=tcp::2222-:22 -daemonize -m 3G -smp cores=4,cpus=4 -drive format=qcow,file=/path/to/your/image
```

4. Finally, you can login into the guest machine with:

```
ssh -p 2222 localhost
```

Another useful step, but not required, it is to install samba server in your
host machine and the client version on the guest machine. With this, you can
have a shared directory between the guest and the host. You can start the guest
with the following command:

```
qemu-system-x86_64 -enable-kvm -net nic -net user,hostfwd=tcp::2222-:22,smb=/path/to/the/shared/directory -daemonize -m 3G -smp cores=4,cpus=4 -drive format=qcow,file=/path/to/your/image
```

And inside the guest machine, you can type:

```
sudo mount -t cifs -o username=yourusername //10.0.2.4/qemu /path/inside/guest
```

Or you can make this task automatically by add the following line into the fstab
file:

```
//10.0.2.4/qemu/ /path/to/shared/directory cifs username=yourusername,credentials=/path/to/.credentialssmb
```

It is totally up to you to keep your custom kernel inside the virtual machine
or shared with your host computer.

# Another approach to compile:

If you are using a distro like Fedora, Debian, Ubuntu or Arch you probably have
some facilities related to kernel compilation and installation using the default
package system of your distro. I personally, dislike this option because I lost
the control of some steps and it is a ditro dependence. However, using the
default package system of your distro can make much easier to add and remove
kernel version and avoid basic mistakes.

As illustration, I will brief show the steps required using Debian:

1. The configuration processes, still the same
2. For compile the kernel use:

```
fakeroot make -j`getconf _NPROCESSORS_ONLN` deb-pkg LOCALVERSION=-add-a-name KDEB_PKGVERSION=1
```

3. At the end of the compilation, take a look at the upper folder and you will
   see a bunch of "*.deb" files"

```
ls ../
```

4. Just install the new version:

```
sudo dpkg -i *.deb
```

5. Finally, update your grub and restart:

```
sudo update-grub2
```

----

# Some extra (cool) informations

## What is the vmlinu[z|x] and bzImage produced after the compilation?

* _vmlinux_ is a binary file statically linked, usually, it is used for
  debugging and to produce the symbol table. The _vm_ in the name came from
  _virtual memory_, and _z_ means compressed (e.g.: gzipped)
* _bzImage_ (big zImage): It was created to overcome the problem of the limited
  space available for compressed kernel. Basically, it splits the kernel over
  discontinuous memory regions

The following links has some extra nice explanation about the aforementioned
files and others:

1.  [Vmlinuz definition](http://www.linfo.org/vmlinuz.html)
2.  [Vmlinuz and bzImage](https://en.wikipedia.org/wiki/Vmlinux)

# Reference

Here you can see some of the references that I used to accomplish this task.

* [Initramfs basics](https://en.wikipedia.org/wiki/Initramfs)
* [Initramfs, practical stuffs](http://nairobi-embedded.org/initramfs_tutorial.html)
* [Great explanation about initramfs](https://landley.net/writing/rootfs-intro.html)
* [How to related with initramfs](https://landley.net/writing/rootfs-howto.html)
* [Nice discussion in stackexchange](https://unix.stackexchange.com/questions/48302/running-bzimage-in-qemu-unable-to-mount-root-fs-on-unknown-block0-0)
* [Kernel Readme](https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git/tree/README?id=refs/tags/v4.3.3)
* [Speedup kernel compilation](http://www.h-online.com/open/features/Good-and-quick-kernel-configuration-creation-1403046.html)
* [Stackoverflow with some tips about speedup kernel compilation](https://stackoverflow.com/questions/23279178/how-to-speed-up-linux-kernel-compilation)
* [SYSTEMMD-BOOT](https://wiki.archlinux.org/index.php/systemd-boot#Adding_boot_entries)
* [cpio tutorial](https://www.gnu.org/software/cpio/manual/html_mono/cpio.html)
* [Busybox, build system](https://busybox.net/FAQ.html#build_system)
