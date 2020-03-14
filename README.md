# gUki

A GTK application to manage [μki](https://github.com/nathanpc/libuki) personal
wikis.

## Building

Building this project is extremely simple. All you have to do is choose which
GTK version you want to use when running `cmake`:

```console
foo@bar:~/dev/gUki$ mkdir build
foo@bar:~/dev/gUki$ cd build
foo@bar:~/dev/gUki/build$ cmake .. -DGTK_VERSION=3
foo@bar:~/dev/gUki/build$ make
```

## Installation

If you want to install this application just follow the commands from the
[Building](#Building) section and at the end run `make install`.

## Requirements

This project can be compiled either with GTK+ 2 or GTK+ 3.

  - [libuki](https://github.com/nathanpc/libuki)
  - GTK+ 2
    - [GTK+ 2](https://www.archlinux.org/packages/extra/x86_64/gtk2/) (2.x)
    - [WebKitGTK+](https://aur.archlinux.org/packages/webkitgtk2/) (2.x)
  - GTK+ 3
    - [GTK+ 3](https://www.archlinux.org/packages/extra/x86_64/gtk3/) (3.x)
    - [WebKit2GTK+](https://www.archlinux.org/packages/extra/x86_64/webkit2gtk/) (4.x)

## License

This project is licensed under the **MIT License**.
