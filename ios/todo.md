# todo


cmake -DCMAKE_TOOLCHAIN_FILE=../iOS_64.cmake  -DCMAKE_IOS_DEVELOPER_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/ -DCMAKE_IOS_SDK_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/  -GXcode ..

修改Project设置：
1. Deployment Target
2. build settings->base SDK macOS
3. target 


查看支持的系统架构
lipo -info xx.a

静态库合并
lipo -create xx1.a xx2.a -output xx.a


