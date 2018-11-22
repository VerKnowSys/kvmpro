# kvmpro - Performance driven, low level processes listing library for HardenedBSD/ FreeBSD.

> Project is developed on and requires 64bit FreeBSD-11-compliant systems.

> It's plausible, that it will work on other POSIX-compliant systems, like (OpenBSD, NetBSD, Darwin) but that's not guaranteed or supported.

> Currently only JSON output format is supported.


## Author:

> Daniel (@dmilith) Dettlaff


### Dependencies:

- HardenedBSD +11-STABLE,

- Modern compiler: Clang +6.x,

- Modern linker: LLD +6.x,

- Kernel Memory Interface system feature: "kvm" (for details: `man kvm` on supported OS)

- Process Information system library: "procstat" (for details: `man procstat` on supported OS)


### Functions exported by this library:

- `const char* get_process_usage(uid_t uid);` - returns JSON list with detailed information
    about all processes, (of given user uid). Including: sockets, TCP, UDP, bound addresses,
    ports, protocols and other details. Full list contains fields: "pid", "ppid", "name", "cmd",
    "rss", "mrss", "runtime", "blk", "blk", "thr", "pri" and "statinfo".

- `const char* get_process_usage_short(uid_t uid);` - returns JSON list with short information
    about all processes (running as given user uid). Short list contains only fields: "cmd",
    "pid", "ppid", "runt", "ioin", "ioout" and "rss".



### Released under multiple licenses:

- [BSD](http://opensource.org/licenses/BSD-2-Clause) license,

- [MIT](http://opensource.org/licenses/MIT) license,

- IDGaF license,
