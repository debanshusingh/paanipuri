#!/bin/sh
cd /Library/Frameworks/Houdini.framework/Versions/14.0.258/Resources/; source houdini_setup; cd -;
hcustom -I./paani/thirdparty/osx/glm/ paani/src/SOP_Paani.C;
cp Build/Intermediates/paani.build/Release/paani.build/Objects-normal/x86_64/*.o paani/src;
/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/clang++ -bundle -flat_namespace -undefined suppress paani/src/utilities.o paani/src/Particle.o paani/src/ParticleSystem.o paani/src/scene.o paani/src/SOP_Paani.o -arch x86_64 -fobjc-gc -I./../thirdparty/osx/glm -framework OpenGL -framework Cocoa -stdlib=libstdc++ -o $HOME/Library/Preferences/houdini/14.0/dso/SOP_Paani.dylib
