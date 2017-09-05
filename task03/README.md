# Task 03 description

----

Now that you have your custom kernel up and running, it's time to modify
it!

The tasks for this round is:
  - take the kernel git tree from Task 02 and modify the Makefile to
    and modify the EXTRAVERSION field.  Do this in a way that the
    running kernel (after modifying the Makefile, rebuilding, and
    rebooting) has the characters "-eudyptula" in the version string.
  - show proof of booting this kernel.  Extra cookies for you by
    providing creative examples, especially if done in intrepretive
    dance at your local pub.
  - Send a patch that shows the Makefile modified.  Do this in a manner
    that would be acceptable for merging in the kernel source tree.
    (Hint, read the file Documentation/SubmittingPatches and follow the
    steps there.)

----

# My full answer

## About EXTRAVERSION inside Makefile

Linux Makefile has a variable named `KERNELVERSION` that determine the current
Linux Version. You can find this variable inside the Makefile and it looks like
this:

```
KERNELVERSION = $(VERSION)$(if $(PATCHLEVEL),.$(PATCHLEVEL)$(if $(SUBLEVEL),.$(SUBLEVEL)))$(EXTRAVERSION)
```

After add an extension, it just add the name "-eudyptula" as a part of the
kernel version. If you enabled it, you have to add "n" to the option
`CONFIG_LOCALVERSION_AUTO`.

## Creating patch file

To send a patch for Linux Kernel, you have to take care of many stuffs to
increase your chances to be accepted. In a few words, I produced the patch by
following the steps below:

1. Install `codespell`
2. Create a new hook inside Linux repository, for after committing. Basically:

* Create a after commit script:

```
cd linux/.git/hooks/
touch post-commit && chmod a+x $_
```

* Inside the `post-commit` script:

```
#!/bin/sh
exec git show --format=email HEAD | ./scripts/checkpatch.pl --strict --codespell
```

3. Added my modified files with `git add -p`
4. Committed with Signed-off-by and the diff inside the commit:
  `git commit -s -v`
5. Finally, generate the ".patch" file:

```
git format-patch -o /tmp/ HEAD^
```

# Reference

* [Detailed about kernel compilation](https://0xax.gitbooks.io/linux-insides/content/Misc/how_kernel_compiled.html)
* [Basic stuffs about Linux patch](https://kernelnewbies.org/FirstKernelPatch)
* [Patch philosophy](https://kernelnewbies.org/PatchPhilosophy)
