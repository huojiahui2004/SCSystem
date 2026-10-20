# SCSystem —— 基于 Qt6 的智慧路口演示系统

一套用 **Qt 6 / C++17 / QML** 编写的智慧路口（Smart Crossing）演示系统。由一个中转服务端和三种客户端角色组成，各端通过 **TCP + JSON** 交换数据，实现车辆行驶上报、信号灯状态同步、路口模式控制与集中监控。

## 系统架构

```
                    ┌──────────────────────────┐
                    │        SCServer          │   TCP 服务端（端口 10086）
                    │  连接管理 / 消息路由转发   │   无界面，纯后台
                    └────┬────────┬────────┬───┘
                 JSON    │        │        │   JSON
              ┌──────────┘        │        └──────────┐
              ▼                   ▼                   ▼
     ┌────────────────┐  ┌────────────────┐  ┌────────────────┐
     │ SCCar  车载端   │  │ SCCrossing 路口 │  │ SCClient 监控端 │
     │ client_type=0  │  │ client_type=1  │  │ client_type=2  │
     └────────────────┘  └────────────────┘  └────────────────┘
      车辆位置/速度/续航     信号灯颜色/路口模式    监控界面 + 下发控制指令
```

服务端只做「注册 + 按消息 `type` 转发」：客户端上线后先发 `type=1000` 注册自己的 `client_type`，之后不同 `type` 的消息被转发给对应角色的客户端。

## 目录结构

```
源码/
├── README.md
└── QT_WorkSpace/
    ├── SCServer/          # 服务端（Qt6 Core + Network，无界面）
    │   ├── net/           # 监听、连接、线程、拆包与路由
    │   ├── log/           # 日志输出
    │   ├── config/        # server_conf.ini 读写
    │   ├── systemcontrol.*
    │   └── main.cpp
    ├── SCClient/          # 监控客户端（QML 界面最丰富）
    │   ├── Main.qml       # 主框架与菜单导航
    │   ├── Map.qml        # 地图 / 车辆定位
    │   ├── Card.qml / CarGroup.qml / CrossingGroup.qml
    │   ├── *MonitorPage.qml / *Set.qml / InfoPage.qml
    │   ├── data/ net/ config/ server/ images/
    │   └── SCClient/      # 早期版本副本（历史留存，见文末说明）
    ├── SCCar/             # 车载端（Qt6 Quick + Network + Multimedia）
    │   ├── Main.qml
    │   ├── data/ net/ config/ images/（含报警提示音 888.wav）
    │   └── systemcontrol.*
    └── SCCrossing/        # 路口设备端（Qt6 Quick + Network）
        ├── Main.qml
        ├── data/ net/ config/ images/
        └── systemcontrol.*
```

## 子工程说明

| 子工程 | 角色 | Qt 模块 | client_type | 说明 |
| --- | --- | --- | --- | --- |
| `SCServer` | 中转服务端 | Core、Network | — | TCP 监听（默认 10086），限制连接数（默认 500），粘包拆分与消息路由转发 |
| `SCClient` | 监控客户端 | Quick、Network | 2 | QML 监控大屏：地图、车辆监控、路口监控、流量监控、车辆/路口/系统设置，并向下发控制指令 |
| `SCCar` | 车载端 | Quick、Network、Multimedia | 0 | 按配置路线行驶，周期上报位置/速度/续航/里程，接收控制指令并播放提示音 |
| `SCCrossing` | 路口设备端 | Quick、Network | 1 | 模拟路口信号灯运行，上报信号灯颜色与路口模式，接收模式设置指令 |

### SCServer
- `net/mytcpserver.*`：接收连接，为每个连接创建 `MyTcpSocket` 并移入独立线程。
- `net/mytcpsocket.*`：`handleData()` 按 `{` / `}` 配对计数，从字节流中切出完整 JSON 帧，解决 TCP 粘包；`handleFrame()` 读取 `type` 后按路由表转发。
- `net/netcontext.*`：单例，持有监听 socket，负责 `startServer(port)` 与最大连接数设置。
- `log/logcontext.*`：单例日志，输出 `[yyyy-MM-dd hh:mm:ss.zzz]:消息` 格式。
- `config/configcontext.*`：`server_conf.ini` 的 `[server]` 组，键 `listen_port`（默认 10086）、`client_limit`（默认 500）。

### SCClient
- 界面由 QML 组成，`Main.qml` 负责主框架与菜单导航（地图 / 车辆监控 / 路口监控 / 流量监控 / 车辆设置 / 路口设置 / 系统设置）。
- `data/datacontext.*`：车辆、道路、信号灯等数据模型，供 QML 直接绑定。
- `net/netcontext.*`：连接服务端；`connectedSlot()` 中发 `type=1000` 注册为 `client_type=2`，断线后每 5 秒自动重连；接收下游数据并转为 Qt 信号。
- 下发控制指令：路口模式、车灯开关、报警灯开关、锁车开关、运行开关。

### SCCar
- `config/configcontext.*`：`car_conf.ini` 的 `[set]` 组，包含 `server_ip` / `server_port`（默认 `127.0.0.1:10086`）、`car_id`、`car_key`、行驶路线 `car_run_list`、`car_speed`、`car_endurance`、`car_mileage`，续航与里程可回写。
- 按路线行驶并周期上报车辆运行信息；`images/888.wav` 为报警提示音。
- 接收服务端下发的运行开关 / 车灯 / 报警灯 / 锁车指令。

### SCCrossing
- `config/configcontext.*`：`crossing_conf.ini` 的 `[set]` 组，含 `server_ip` / `server_port`、`crossing_id`、四个方向（上/下/左/右）的 `*_light_id`、`*_light_show`、`*_light_road_id`，以及绿灯时长 `green_time`。
- 上报信号灯颜色变化与路口模式变化；接收服务端下发的路口模式设置。

## 通信协议

- 传输层：TCP，UTF-8 编码的 JSON 对象，**无长度前缀**，收发双方均按 `{` / `}` 配对切帧。
- 每条消息都带 `type`（int）字段；注册消息额外带 `client_type`。

| type | 发送方 | 主要内容字段 | 服务端转发目标 | 说明 |
| --- | --- | --- | --- | --- |
| `1000` | 各客户端 | `client_type` | 不转发 | 上线注册身份（0=车载端，1=路口端，2=监控端） |
| `2001`–`2008` | `SCCar` | `id`、`road_id`、`road_pos`、`speed`、`endurance`、`mileage` | 0、2、3 类客户端 | 车辆运行信息上报（`2008` 为运行位置与状态） |
| `3001` | `SCCrossing` | `light_id`、`road_id`、`color` | 0、2、3 类客户端 | 信号灯颜色变化 |
| `4001` | `SCCrossing` | `crossing_id`、`mode` | 2、3 类客户端 | 路口模式变化 |
| `5001` | `SCClient` | `id`、`sw`、`key` | 0 类客户端 | 车辆运行开关 |
| `5002` | `SCClient` | `id`、`sw`、`key` | 0 类客户端 | 车灯开关 |
| `5003` | `SCClient` | `id`、`sw`、`key` | 0 类客户端 | 报警灯开关 |
| `5004` | `SCClient` | `id`、`sw`、`key` | 0 类客户端 | 锁车开关 |
| `6001` | `SCClient` | `id`、`mode` | 1 类客户端 | 下发路口模式 |

消息样例：

```json
{ "type": 1000, "client_type": 2 }
{ "type": 2008, "id": 0, "road_id": 3, "road_pos": 12, "speed": 5, "endurance": 988.0, "mileage": 12.0 }
{ "type": 5002, "id": 0, "sw": true, "key": "ADMIN123" }
```

## 编译与运行

环境要求：

- Qt **6.8 及以上**（含 Quick、Network、Multimedia；`SCServer` 最低要求 6.5）
- CMake 3.16 以上（`SCServer` 要求 3.19 以上）
- 支持 C++17 的编译器（MSVC / MinGW / GCC / Clang）

Qt Creator 打开：分别打开各子工程目录下的 `CMakeLists.txt`，选择 Kit 后构建运行。

命令行构建（以服务端为例）：

```bash
cd QT_WorkSpace/SCServer
cmake -B build -DCMAKE_PREFIX_PATH=<Qt安装路径>
cmake --build build
```

建议启动顺序：先启动 `SCServer`，再启动 `SCCrossing`、`SCCar`、`SCClient`，随后各端会以 `127.0.0.1:10086` 自动连接并注册。

## 注意事项

- 各端默认连接 `127.0.0.1:10086`，实际部署时请修改各自的 `*.ini` 配置：服务端 `server_conf.ini`、车载端 `car_conf.ini`、路口端 `crossing_conf.ini`。
- 车载端默认密钥 `ADMIN123` 仅用于演示，正式使用请替换。
- `build*/`、`.moc/`、`*.exe`、`*.qmlls.ini` 等生成文件已由各子工程的 `.gitignore` 排除。
- `QT_WorkSpace/SCClient/SCClient/` 是监控客户端的早期版本副本（界面文件较少），仅作历史留存，当前有效版本为 `QT_WorkSpace/SCClient/`。
- 监控客户端的 `SCClient/config/` 下仍是服务端用的 `server_conf.ini` 配置代码（未被 `CMakeLists.txt` 引用）；其服务端地址与端口在 `main.cpp` 中直接指定。
