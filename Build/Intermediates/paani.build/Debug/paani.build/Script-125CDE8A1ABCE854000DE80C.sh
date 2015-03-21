#!/bin/sh
cd /Library/Frameworks/Houdini.framework/Versions/14.0.258/Resources/; source houdini_setup; cd -;hcustom -I./paani/thirdparty/osx/glm/ paani/src/SOP_Paani.C;
