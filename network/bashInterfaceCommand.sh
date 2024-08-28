#!/bin/bash
#交互命令
#创建robot信息
bash ./interface_ex.sh createAsset -peer robot1 -posx 2.3 -posy 2.5 -speed 20
bash ./interface_ex.sh createAsset -peer robot2 -posx 2.105 -posy 2.203 -speed 50
#docker exec cli bash -c "$Org1Peer4Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"createAsset\",\"robot4\",\"2.105\",\"2.203\",\"50\"]}'"
#计算可靠性
bash ./interface_ex.sh caculateReliability -peer robot1
#查询robot的速度信息
bash ./interface_ex.sh queryRobotspeed -peer robot1
#获取平均速度信息
bash ./interface_ex.sh queryAveRobotspeed -peer robot1
