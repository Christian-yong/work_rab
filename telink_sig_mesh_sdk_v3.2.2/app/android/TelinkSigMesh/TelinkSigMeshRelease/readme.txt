
tip: 1. 在windows上导入并编译项目时， 如果有 too many path components 这样的error信息， 需要更新ninja工具。
         压缩包内已经有测试可用的工具， 客户也可以在 https://github.com/ninja-build/ninja/releases 地址下载新版;


version info

V3.1.6
1. TTL 由 1 改为 10;

==================================================================

V3.1.3
1. 修复进入remote provision页面是概率性闪退问题;
2. 修复对remote scan无设备返回问题;

==================================================================


V3.1.1
1. 移除 MeshOTAModelStorage 中的 rsv字段;
2. 更新lib

==================================================================

V3.1.0
1. LPN 节点的控制: 添加开关灯等功能支持;
2. mesh.json更新: provisioner AddressRange 由256改为1024
3. 加灯完成后如果设备包含time model则添加time publish， 且在自动连接完成后发送time status;
4. 更新lib

==================================================================

V3.0.0
1. 添加autoConnect模式下nodeIdentity/networkId识别;
2. 添加fastProvision组网功能;

==================================================================

V2.9.0
1. 修复MeshOTA中出现的状态限制错误;
2. 移除TelinkSigMeshLib中的flash文件及其相关操作代码，并添加数据存储/恢复接口;
3. 添加heart beat状态上报接口;
4. 更新静态库;

==================================================================

V2.8.2
1. HSL设备控制页面添加RGB调节滑杆;
2. 添加Mesh OTA功能， 通过setting->Mesh OTA可以进入相关页面;
3. 修复app重启后， 不能分组和踢灯问题;

==================================================================

V2.8.1

1. 添加online status, app会根据对应的UUID判断是否打开online status 功能;
2. demo中添加权限校验
3. 更新remote provision功能
4. 在windows上导入并编译项目时， 如果有 too many path components 这样的error信息， 需要更新ninja工具。
    压缩包内已经有测试可用的工具， 客户也可以在 https://github.com/ninja-build/ninja/releases 地址下载新版;

==================================================================

V2.8.0
1. 移除auto connect 模式下的自动获取设备状态动作， 改为在demo中监听login事件， 并添加对应的动作。
    具体参考MainActivity#performed#MeshEvent.EVENT_TYPE_AUTO_CONNECT_LOGIN 事件处理;
2. 添加filter init confirm 机制；
3. 移除 mesh.json 默认保存， 仅保留import/export功能;
4. 添加lpn 节点显示， 在LPN设备页面可以通过发送vendor指令获取对应状态信息;
5. 添加Private Mode, 该模式打开时，会对扫描到的unprovisioned 设备 过滤， 符合过滤条件的会进行fast bind 操作;
6. 添加Remote Provision 模式, 该模式打开时，
7. 将原有的cmd接口标注deprecated， 改为统一ini发送指令入口， 建议后续都采用MeshService#sendMeshCommand接口发送控制指令;

==================================================================

V2.7.0
1. 更新Lib
2. 添加Json格式存储， 添加网络恢复的相关接口(reattachNodes, resetAppKey), 且可以通过Json共享iOS与vc端app数据；移除本地Mesh文件;
3. 添加分享页面, 包含import和export子页面;
4. 更新setting页面: 添加Log enable选项， 打开后使能log功能；显示app key 和 net key;

==================================================================

update 2018/12/20(V2.6.1)
1. 添加Json格式支持， 添加网络恢复的相关接口(reattachNodes, resetAppKey)
2. 移除本地Mesh文件

==================================================================

update 2018/11/02(V2.6.0)
1. 添加场景和闹钟功能；
2. 添加多element支持；
3. 添加level控制功能；
4. 添加设备订阅状态配置功能；
5. UI更新， 保持整体与iOS一致；

==================================================================

update 2018/07/20(V2.5.0)
1. appKey bind时， 由默认添加10个model改为默认添加所有model；
2. 添加vendor指令发送，与vendor rsp接收
3. 更新 kickout 功能, 添加kickout对应事件回调
4. 新增 color panel
5. 更新 lib

==================================================================

update 2018/07/20(V2.4.0)
1. 更新Lib;
2. 更改so打包方式， 由原有ndk-build 改为 cmake， 配置文件为lib/CMakeLists.txt
3. 更新provision接口， 更新扫描接口

==================================================================

update 2018/06/22(V2.3.0)
1. 更新Lib

==================================================================


update 2018/06/08(V2.2.0)
1. SDK中分离key bind 与provision过程， 并提供独立的key bind接口
2. app中添加设备key bind不成功标志， 并可以由用户选择重新进入key bind流程
3. 主页中新增Setting分栏， 提供model显示与选择

==================================================================

update 2018/06/02(V2.0.3)
1. 开放jni源码
2. 实现cloud操作相关接口

==================================================================

update 2018/04/01 (V2.0.2)
1. jni 修复反复调用接口造成的溢出问题
2. lib更新

==================================================================

update 2018/04/01 (V2.0.1)
1. provision 过程中添加bind操作，成功标记改为bind执行完成;
2. 添加色温控制指令与色温相关解析;
3. meshLib更新;

==================================================================

update 2018/01/31 (V2.0.0)
1. 更新MeshLib
2. 自动连接过程中添加【初始化白名单】操作

==================================================================

update 2017/11/30 (V1.1.0)
1. kick_out更新： 在剔除直连灯时，会等待连接断开;
2. 代码结构修改， 将原有的App分离为 demo + lib;
3. 自动连接时， 添加rssi 判断: 默认扫描到设备后，收集3s内的设备rssi值，取最大平均值的设备发起连接；

==================================================================

update 2017/11/17 (V1.0.4)

1. kick_out功能更新： 在执行删灯操作时，会删除本地数据并发送删灯指令；

2. ota页面增加固件版本信息；

3. 加灯过程中增加多次连接失败的处理；

3. 更新MeshLib, so文件打包方式更新；

==================================================================

update 2017/11/07 (V1.0.3)

1. 在主页自动连接成功后 获取online_status 状态指令中的 [rsp_max] 参数 由 [加过的灯的数量] 改为 [0]，以提高重连后的响应速度；

2. 增加设备的kick_out 操作， 操作后该设备相关数据会从app本地删除；

3. 首页添加app版本号信息;

4. 更新MeshLib;

==================================================================

update 2017/10/30  (V1.0.2)

1. MeshLib 与VC 端同步更新;

2. 增加指令busy状态;

3. OTA 发包结束后不主动断连;

==================================================================

update 2017/09/20  (V1.0.1)

1. 修复MeshLib 多次调用initAll 导致online_status 不返回的问题；

2. 连接过程中连接成功后的wait时间由3000ms 改为500ms；

3. 连接过程中添加10s连接超时判断；

4. 加灯过程中会过滤掉已加过的灯；
