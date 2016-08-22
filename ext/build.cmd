cl /I..\..\Inc;..\..\Extensions /Ox %1 testavx.cpp /arch:AVX
cl /I..\..\Inc;..\..\Extensions /Ox %1 testavx2.cpp /arch:AVX2
cl /I..\..\Inc;..\..\Extensions /Ox %1 testbe.cpp
cl /I..\..\Inc;..\..\Extensions /Ox %1 testf16c.cpp /arch:AVX
cl /I..\..\Inc;..\..\Extensions /Ox %1 testfma3.cpp /arch:AVX
cl /I..\..\Inc;..\..\Extensions /Ox %1 testfma4.cpp /arch:AVX
cl /I..\..\Inc;..\..\Extensions /Ox %1 testsse3.cpp
cl /I..\..\Inc;..\..\Extensions /Ox %1 testsse4.cpp