/* Dummy crtn file.

   In this configuration, crti.o and crtn.o are both empty because the
   .init_array/.fini_array sections are used exclusively.

   New configurations without compatibility concerns for
   toolchains without .init_array support can use this to avoid the
   superfluous .init and .fini boilerplate code.  */