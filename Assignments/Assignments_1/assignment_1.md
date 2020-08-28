# Assignment 1

#### Shuyang Cao

## Problem 1

Code for this problem.
```cpp
/*
    exp0.cpp     empty program
*/
int main(int argc, char** argv)
{
    return 0;
}


/*
    exp1.cpp     exp() added with a constant argument
*/
#include<cmath>

int main(int argc, char **argv)
{
    double a = std::exp(2.0);
    return 0;
}


/*
    exp2.cpp    exp() added with a variable argument
*/
#include<cmath>

int main(int argc, char **argv)
{
    double a = 2;
    double b = std::exp(a);
    return 0;
}

/*
    exp3.cpp    cout added
*/
#include<iostream>
#include<cmath>

int main(int argc, char **argv)
{
    std::cout << std::exp(2.0) << std::endl;
    return 0;
}
```

Using exp() as an example for this problem is really **BAD** idea for the following reasons.

First let's check output from ldd first.
```bash
$ ldd exp0
	linux-vdso.so.1 (0x00007ffdb9fbb000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007faff00ec000)
	/lib64/ld-linux-x86-64.so.2 (0x00007faff02f6000)

$ ldd exp1
    linux-vdso.so.1 (0x00007ffcd87bd000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fa2489ef000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fa248bf9000)

$ ldd exp2
	linux-vdso.so.1 (0x00007fff97def000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fc3a0276000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fc3a0084000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fc3a03dd000)

$ ldd exp3
	linux-vdso.so.1 (0x00007ffebfd66000)
	libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007fb47d127000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fb47cf35000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007fb47cde6000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fb47d320000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007fb47cdcb000)
```

The `main` function is not the line executed when a program is run. It's not even the first routine executed in a program. Some code is needed for creating process, setting up runtime enviroment for a program to live in. So we find that `libc.so` is needed even in an empty program.

The comes an issue. We anticipate `libm.so` is needed in `exp1.cpp` as in `exp2.cpp` since both of them invoke `exp()`. However `libm.so` is not linked until `exp2.cpp`. I guess this is due to the default optimization of GCC. In `exp1.cpp`, a constant is provided for `exp()`, which suggests that the result of `exp()` can be calculated at compiling time instead of runtime. Thus the invoke of `exp()` is optimized out. Actually if we set the optimization level high enough, `exp()` in `exp2.cpp` could also be optimized out.

Then we investigate the output from `file`. The four output are the same except their names and their SHA1 signatures since they are compiled on the same platform using the same compiler configuration. So we use the output of `exp0.cpp` as an exmaple.

```bash
$ file exp0
exp0: ELF 64-bit LSB shared object, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=fd49c4ade8c10780e6973b6736ffcbe1317c842f, for GNU/Linux 3.2.0, not stripped
```

Next we take a look at the output of `nm exp* | c++filt`.

```bash
$ nm exp0 | c++filt
0000000000004010 B __bss_start
0000000000004010 b completed.8059
                 w __cxa_finalize@@GLIBC_2.2.5
0000000000004000 D __data_start
0000000000004000 W data_start
0000000000001070 t deregister_tm_clones
00000000000010e0 t __do_global_dtors_aux
0000000000003df8 d __do_global_dtors_aux_fini_array_entry
0000000000004008 D __dso_handle
0000000000003e00 d _DYNAMIC
0000000000004010 D _edata
0000000000004018 B _end
00000000000011b8 T _fini
0000000000001120 t frame_dummy
0000000000003df0 d __frame_dummy_init_array_entry
000000000000212c r __FRAME_END__
0000000000003fc0 d _GLOBAL_OFFSET_TABLE_
                 w __gmon_start__
0000000000002004 r __GNU_EH_FRAME_HDR
0000000000001000 t _init
0000000000003df8 d __init_array_end
0000000000003df0 d __init_array_start
0000000000002000 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
00000000000011b0 T __libc_csu_fini
0000000000001140 T __libc_csu_init
                 U __libc_start_main@@GLIBC_2.2.5
0000000000001129 T main
00000000000010a0 t register_tm_clones
0000000000001040 T _start
0000000000004010 D __TMC_END__
```

As we can see, our `main()` function doesn't appear until the last fourth row. All rows before the "main" symbol are setup code mentioned above. Since the setup procedure are the same for most programs, we only focus on the part starting from `main` below.

```bash
$ nm exp1 | c++filt
0000000000001129 T main
00000000000010a0 t register_tm_clones
0000000000001040 T _start
0000000000004010 D __TMC_END__

$ nm exp2 | c++filt
0000000000001149 T main
00000000000010c0 t register_tm_clones
0000000000001060 T _start
0000000000004010 D __TMC_END__

$ nm exp3 | c++filt
00000000000011a9 T main
0000000000001120 t register_tm_clones
00000000000010c0 T _start
0000000000004010 D __TMC_END__
00000000000011f0 t __static_initialization_and_destruction_0(int, int)
                 U std::basic_ostream<char, std::char_traits<char> >::operator<<(double)@@GLIBCXX_3.4
                 U std::basic_ostream<char, std::char_traits<char> >::operator<<(std::basic_ostream<char, std::char_traits<char> >& (*)(std::basic_ostream<char, std::char_traits<char> >&))@@GLIBCXX_3.4
                 U std::ios_base::Init::Init()@@GLIBCXX_3.4
                 U std::ios_base::Init::~Init()@@GLIBCXX_3.4
0000000000004040 B std::cout@@GLIBCXX_3.4
                 U std::basic_ostream<char, std::char_traits<char> >& std::endl<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&)@@GLIBCXX_3.4
0000000000002008 r std::piecewise_construct
0000000000004151 b std::__ioinit
```

It should not be surprising that the output of `exp1.cpp` is the same as `exp0.cpp` if we agree that the invoke of `exp()` is optimized out in `exp1.cpp`. The problem is that we can't find function invocation in `exp2.cpp`, either. I guess this is due to the implemention of `exp()`, which is shown below.

```cpp
#ifndef __CORRECT_ISO_CPP_MATH_H_PROTO
    inline _GLIBCXX_CONSTEXPR float
    exp(float __x)
    { return __builtin_expf(__x); }
    inline _GLIBCXX_CONSTEXPR long double
    exp(long double __x)
    { return __builtin_expl(__x); }
#endif
```

`exp()` is defined to be an inline function, which means the preprocessor will unfold `exp()` first and the compiler doesn't see any function invocation here. It's highly probable that `exp()` is implemented in assembly code for performance so that there is any symbol there. In `exp3.cpp`, `cout` obviously cannot be implemented as an inline function. Hence, we are able to find some function invocations in the output.

Finally we have a look at the effect of `nm`. We take the output from `exp3.cpp` as our example. Again, we omit the part for setup code.

```bash
$ nm exp3
00000000000011a9 T main
0000000000001120 t register_tm_clones
00000000000010c0 T _start
0000000000004010 D __TMC_END__
00000000000011f0 t _Z41__static_initialization_and_destruction_0ii
                 U _ZNSolsEd@@GLIBCXX_3.4
                 U _ZNSolsEPFRSoS_E@@GLIBCXX_3.4
                 U _ZNSt8ios_base4InitC1Ev@@GLIBCXX_3.4
                 U _ZNSt8ios_base4InitD1Ev@@GLIBCXX_3.4
0000000000004040 B _ZSt4cout@@GLIBCXX_3.4
                 U _ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@@GLIBCXX_3.4
0000000000002008 r _ZStL19piecewise_construct
0000000000004151 b _ZStL8__ioinit

$ nm exp3 | c++filt
00000000000011a9 T main
0000000000001120 t register_tm_clones
00000000000010c0 T _start
0000000000004010 D __TMC_END__
00000000000011f0 t __static_initialization_and_destruction_0(int, int)
                 U std::basic_ostream<char, std::char_traits<char> >::operator<<(double)@@GLIBCXX_3.4
                 U std::basic_ostream<char, std::char_traits<char> >::operator<<(std::basic_ostream<char, std::char_traits<char> >& (*)(std::basic_ostream<char, std::char_traits<char> >&))@@GLIBCXX_3.4
                 U std::ios_base::Init::Init()@@GLIBCXX_3.4
                 U std::ios_base::Init::~Init()@@GLIBCXX_3.4
0000000000004040 B std::cout@@GLIBCXX_3.4
                 U std::basic_ostream<char, std::char_traits<char> >& std::endl<char, std::char_traits<char> >(std::basic_ostream<char, std::char_traits<char> >&)@@GLIBCXX_3.4
0000000000002008 r std::piecewise_construct
0000000000004151 b std::__ioinit
```
As shown above, `nm` mangles function names and `c++filt` reverses it.

## Problem 2

The code is under "CH1/EX1".

```bash
$ ldd exp
	linux-vdso.so.1 (0x00007fff3e19e000)
	libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f25f3f76000)
	libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f25f3e27000)
	libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f25f3e0c000)
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f25f3c1a000)
	/lib64/ld-linux-x86-64.so.2 (0x00007f25f4172000)

$ nm exp | c++filt

# The output is omitted since it is similar the that in EX1 except it's much longer.
```