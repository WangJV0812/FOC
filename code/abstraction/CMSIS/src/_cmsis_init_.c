/* Empty init definition to avoid errors linking libc.a */
/* it is not elegant, but effective*/

// FIXME： it is unelegant, try to link libc.a in a correct way
__attribute__((weak)) void _close_r(void){}
__attribute__((weak)) void _fstat_r(void){}
__attribute__((weak)) void _getpid_r(void){}
__attribute__((weak)) void _isatty_r(void){}
__attribute__((weak)) void _kill_r(void){}
__attribute__((weak)) void _lseek_r(void){}
__attribute__((weak)) void _open_r(void){}
__attribute__((weak)) void _read_r(void){}
__attribute__((weak)) void _write_r(void){}
__attribute__((weak)) void _getentropy_r(void){}
