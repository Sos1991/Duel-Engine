bjam.exe --toolset=msvc-10.0 --stagedir=./vc10_x86 --builddir=./ address-model=32 --with-date_time --with-system --with-thread link=shared runtime-link=shared threading=multi define=_CRT_SECURE_NO_DEPRECATE define=_SCL_SECURE_NO_DEPRECATE stage debug release
bjam.exe --toolset=msvc-10.0 --stagedir=./vc10_x64 --builddir=./ address-model=64 --with-date_time --with-system --with-thread link=shared runtime-link=shared threading=multi define=_CRT_SECURE_NO_DEPRECATE define=_SCL_SECURE_NO_DEPRECATE stage debug release