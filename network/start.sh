#!/bin/bash

# 检查操作系统类型
if [[ `uname` == 'Darwin' ]]; then
  echo "当前操作系统是 Mac"
  export PATH=${PWD}/hyperledger-fabric-darwin-amd64-1.4.12/bin:$PATH
elif [[ `uname` == 'Linux' ]]; then
  echo "当前操作系统是 Linux"
  export PATH=${PWD}/hyperledger-fabric-linux-amd64-1.4.12/bin:$PATH
else
  echo "当前操作系统不是 Mac 或 Linux，脚本无法继续执行！"
  exit 1
fi

echo -e "注意：倘若您之前已经部署过了 network ，执行该脚本会丢失之前的数据！\n若只是想重启 network ，可以直接执行 docker-compose restart 命令。"
read -p "你确定要继续执行吗？请输入 Y 或 y 继续执行：" confirm

if [[ "$confirm" != "Y" && "$confirm" != "y" ]]; then
  echo "你取消了脚本的执行。"
  exit 1
fi

echo "一、清理环境"
./stop.sh

echo "二、生成证书和秘钥（ MSP 材料），生成结果将保存在 crypto-config 文件夹中"
cryptogen generate --config=./crypto-config.yaml

echo "三、创建排序通道创世区块"
configtxgen -profile TwoOrgsOrdererGenesis -outputBlock ./config/genesis.block -channelID firstchannel

echo "四、生成通道配置事务'appchannel.tx'"
configtxgen -profile TwoOrgsChannel -outputCreateChannelTx ./config/appchannel.tx -channelID appchannel

echo "五、为 Taobao 定义锚节点"
configtxgen -profile TwoOrgsChannel -outputAnchorPeersUpdate ./config/Org1Anchor.tx -channelID appchannel -asOrg Org1

echo "六、为 JD 定义锚节点"
configtxgen -profile TwoOrgsChannel -outputAnchorPeersUpdate ./config/Org2Anchor.tx -channelID appchannel -asOrg Org2

echo "区块链 ： 启动"
docker-compose up -d
echo "正在等待节点的启动完成，等待10秒"
sleep 10

Org1Peer0Cli="CORE_PEER_ADDRESS=peer0.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
Org1Peer1Cli="CORE_PEER_ADDRESS=peer1.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
Org1Peer2Cli="CORE_PEER_ADDRESS=peer2.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
Org1Peer3Cli="CORE_PEER_ADDRESS=peer3.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
Org1Peer4Cli="CORE_PEER_ADDRESS=peer4.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
Org1Peer5Cli="CORE_PEER_ADDRESS=peer5.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
Org1Peer6Cli="CORE_PEER_ADDRESS=peer6.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
Org1Peer7Cli="CORE_PEER_ADDRESS=peer7.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
Org1Peer8Cli="CORE_PEER_ADDRESS=peer8.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
Org2Peer0Cli="CORE_PEER_ADDRESS=peer0.org2.example.com:7051 CORE_PEER_LOCALMSPID=Org2MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org2.example.com/users/Admin@org2.example.com/msp"

echo "七、创建通道"
docker exec cli bash -c "$Org1Peer0Cli peer channel create -o orderer.example.com:7050 -c appchannel -f /etc/hyperledger/config/appchannel.tx"

echo "八、将所有节点加入通道"
docker exec cli bash -c "$Org1Peer0Cli peer channel join -b appchannel.block"
docker exec cli bash -c "$Org1Peer1Cli peer channel join -b appchannel.block"
docker exec cli bash -c "$Org1Peer2Cli peer channel join -b appchannel.block"
docker exec cli bash -c "$Org1Peer3Cli peer channel join -b appchannel.block"
docker exec cli bash -c "$Org1Peer4Cli peer channel join -b appchannel.block"
docker exec cli bash -c "$Org1Peer5Cli peer channel join -b appchannel.block"
docker exec cli bash -c "$Org1Peer6Cli peer channel join -b appchannel.block"
docker exec cli bash -c "$Org1Peer7Cli peer channel join -b appchannel.block"
docker exec cli bash -c "$Org1Peer8Cli peer channel join -b appchannel.block"
docker exec cli bash -c "$Org2Peer0Cli peer channel join -b appchannel.block"

echo "九、更新锚节点"
docker exec cli bash -c "$Org1Peer0Cli peer channel update -o orderer.example.com:7050 -c appchannel -f /etc/hyperledger/config/Org1Anchor.tx"
docker exec cli bash -c "$Org2Peer0Cli peer channel update -o orderer.example.com:7050 -c appchannel -f /etc/hyperledger/config/Org2Anchor.tx"

# -n 链码名，可以自己随便设置
# -v 版本号
# -p 链码目录，在 /opt/gopath/src/ 目录下
echo "十、安装链码"
docker exec cli bash -c "$Org1Peer0Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"
docker exec cli bash -c "$Org1Peer1Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"
docker exec cli bash -c "$Org1Peer2Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"
docker exec cli bash -c "$Org1Peer3Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"
docker exec cli bash -c "$Org1Peer4Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"
docker exec cli bash -c "$Org1Peer5Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"
docker exec cli bash -c "$Org1Peer6Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"
docker exec cli bash -c "$Org1Peer7Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"
docker exec cli bash -c "$Org1Peer8Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"
docker exec cli bash -c "$Org2Peer0Cli peer chaincode install -n fabric-realty -v 1.0.0 -l golang -p chaincode"

# 只需要其中一个节点实例化
# -n 对应上一步安装链码的名字
# -v 版本号
# -C 是通道，在fabric的世界，一个通道就是一条不同的链
# -c 为传参，传入init参数
echo "十一、实例化链码"
docker exec cli bash -c "$Org1Peer0Cli peer chaincode instantiate -o orderer.example.com:7050 -C appchannel -n fabric-realty -l golang -v 1.0.0 -c '{\"Args\":[\"init\"]}' -P \"OR ('Org1MSP.member','Org2MSP.member')\""

echo "正在等待链码实例化完成，等待5秒"
sleep 5

# 进行链码交互，验证链码是否正确安装及区块链网络能否正常工作
echo "十二、验证链码"
docker exec cli bash -c "$Org1Peer0Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"hello\"]}'"
#先初始化一下，不然直接存的时候太慢了
#docker exec cli bash -c "$Org1Peer1Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"caculateReliability\"]}'"

docker exec cli bash -c "$Org1Peer0Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"createAsset\",\"robot1\",\"0\",\"0\",\"0\"]}'"

docker exec cli bash -c "$Org1Peer1Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"createAsset\",\"robot1\",\"0\",\"0\",\"0\"]}'"

docker exec cli bash -c "$Org1Peer2Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"createAsset\",\"robot2\",\"0\",\"0\",\"0\"]}'"

docker exec cli bash -c "$Org1Peer3Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"createAsset\",\"robot3\",\"0\",\"0\",\"0\"]}'"

docker exec cli bash -c "$Org1Peer4Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"createAsset\",\"robot4\",\"0\",\"0\",\"0\"]}'"

docker exec cli bash -c "$Org1Peer5Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"createAsset\",\"robot5\",\"0\",\"0\",\"0\"]}'"

docker exec cli bash -c "$Org1Peer6Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"createAsset\",\"robot6\",\"0\",\"0\",\"0\"]}'"

#docker exec cli bash -c "$Org1Peer0Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"queryAsset\",\"robot1\"]}'"

if docker exec cli bash -c "$Org2Peer0Cli peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"hello\"]}'" 2>&1 | grep "Chaincode invoke successful"; then
  echo "【恭喜您！】 network 部署成功，后续如需暂时停止运行，可以执行 docker-compose stop 命令（不会丢失数据）。"
  exit 0
fi

echo "【警告】network 未部署成功，请检查每一个步骤，定位具体问题。"
