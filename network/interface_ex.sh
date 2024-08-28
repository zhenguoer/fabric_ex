#!/bin/bash
#export FABRIC_CFG_PATH=../hyperledger/config/
#export CORE_PEER_LOCALMSPID=Org1MSP
#export CORE_PEER_TLS_ENABLED=true
#export CORE_PEER_TLS_ROOTCERT_FILE=../network/organizations/peerOrganizations/org1.example.com/tlsca/tlsca.org1.example.com-cert.pem
#export CORE_PEER_MSPCONFIGPATH=../network/organizations/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
#export PATH=$PATH:../hyperledger/bin


export Org1Peer0Cli="CORE_PEER_ADDRESS=peer0.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
export Org1Peer1Cli="CORE_PEER_ADDRESS=peer1.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
export Org1Peer2Cli="CORE_PEER_ADDRESS=peer2.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
export Org1Peer3Cli="CORE_PEER_ADDRESS=peer3.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
export Org1Peer4Cli="CORE_PEER_ADDRESS=peer4.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
export Org1Peer5Cli="CORE_PEER_ADDRESS=peer5.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
export Org1Peer6Cli="CORE_PEER_ADDRESS=peer6.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
export Org1Peer7Cli="CORE_PEER_ADDRESS=peer7.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
export Org1Peer8Cli="CORE_PEER_ADDRESS=peer8.org1.example.com:7051 CORE_PEER_LOCALMSPID=Org1MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org1.example.com/users/Admin@org1.example.com/msp"
export Org2Peer0Cli="CORE_PEER_ADDRESS=peer0.org2.example.com:7051 CORE_PEER_LOCALMSPID=Org2MSP CORE_PEER_MSPCONFIGPATH=/etc/hyperledger/peer/org2.example.com/users/Admin@org2.example.com/msp"

#function setPeer(){
#    if [ "$PEER_NUMBER" == "robot0" ] ; then
#        export CORE_PEER_ADDRESS=localhost:7051
#    elif [ "$PEER_NUMBER" == "robot1" ] ; then
#        export CORE_PEER_ADDRESS=localhost:7052
#    elif [ "$PEER_NUMBER" == "robot2" ] ; then
#        export CORE_PEER_ADDRESS=localhost:7055
#    else
#        echo "Invalid robot ID. Setting as localhost:7051"
#        PEER_NUMBER="robot0"
#        export CORE_PEER_ADDRESS=localhost:7051
#    fi
#}

#function getAllAssets() {
#setPeer
#echo "Getting ledger of $PEER_NUMBER, $CORE_PEER_ADDRESS"
#peer chaincode query -C mychannel -n basic -c '{"Args":["GetAllAssets"]}' | jq .

#}

#function updateAsset() {
#    setPeer
#    peer chaincode invoke -o localhost:7050 --ordererTLSHostnameOverride orderer.example.com --tls --cafile "../network/organizations/ordererOrganizations/example.com/orderers/orderer.example.com/msp/tlscacerts/tlsca.example.com-cert.pem" -C mychannel -n basic -c "{\"function\":\"UpdateAsset\",\"Args\":[\"$LANDMARK\", \"$POSITION_X\", \"$POSITION_Y\", \"$REPORTED_BY\"]}" 
#}

#function initAsset() {
#    setPeer
#   peer chaincode invoke -o localhost:7050 --ordererTLSHostnameOverride orderer.example.com --tls --cafile "../network/organizations/ordererOrganizations/example.com/orderers/orderer.example.com/msp/tlscacerts/tlsca.example.com-cert.pem" -C mychannel -n basic -c "{\"function\":\"InitLedger\",\"Args\":[]}" 
#}

#function channelList() {
#  setPeer
#  peer channel list
#}

#function blockchainInfo() {
#  setPeer
#  peer channel fetch 0 /dev/null >&-  # Fetch the genesis block

#channels=$(grep -oP 'Received block.*channel [^ ]+' peer.log | awk '{print $NF}' | sort -u)
#number_of_peers=$(echo "$channels" | wc -l)

#echo "Number of peers: $number_of_peers"
#}

function setPeer(){
  if [ "$PEER_NUMBER" == "robot0" ] ; then
        CORE_PEER="$Org1Peer0Cli"
elif [ "$PEER_NUMBER" == "robot1" ] ; then
        CORE_PEER="$Org1Peer1Cli"
elif [ "$PEER_NUMBER" == "robot2" ] ; then
        CORE_PEER="$Org1Peer2Cli"
elif [ "$PEER_NUMBER" == "robot3" ] ; then
        CORE_PEER="$Org1Peer3Cli"
elif [ "$PEER_NUMBER" == "robot4" ] ; then
        CORE_PEER="$Org1Peer4Cli"
elif [ "$PEER_NUMBER" == "robot5" ] ; then
        CORE_PEER="$Org1Peer5Cli"
elif [ "$PEER_NUMBER" == "robot6" ] ; then
        CORE_PEER="$Org1Peer6Cli"
elif [ "$PEER_NUMBER" == "robot7" ] ; then
        CORE_PEER="$Org1Peer7Cli"
elif [ "$PEER_NUMBER" == "robot8" ] ; then
        CORE_PEER="$Org1Peer8Cli"
   else
        echo "Invalid robot ID. Setting as Org1Peer0Cli"
        CORE_PEER="Org1Peer0Cli"
    fi
}



function CaculateReliability() {
    setPeer
 #执行命令，并将标准错误输出重定向到标准输出
 result=$(docker exec cli bash -c "$CORE_PEER peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"caculateReliability\"]}'" 2>&1 | grep --color=never -oP 'payload:"\K[^"]+')
 echo "$result"
}


function CreateAsset(){
  setPeer
  docker exec cli bash -c "$CORE_PEER peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"createAsset\",\"$PEER_NUMBER\",\"$POSITION_X\",\"$POSITION_Y\",\"$Speed\"]}'"
}

function QueryRobotSpeed(){
  setPeer
  sped=$(docker exec cli bash -c "$CORE_PEER peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"queryRobotSpeed\",\"$PEER_NUMBER\"]}'" 2>&1 | grep --color=never -oP 'payload:"\K[^"]+')
  echo "$sped"
}

function QueryAveRobotSpeed(){
  setPeer
  ave=$(docker exec cli bash -c "$CORE_PEER peer chaincode invoke -C appchannel -n fabric-realty -c '{\"Args\":[\"queryAveRobotSpeed\"]}'" 2>&1 | grep --color=never -oP 'payload:"\K[^"]+')
   echo "$ave"
}


if [[ $# -lt 1 ]] ; then
  exit 0
else
  MODE=$1
  shift
fi

while [[ $# -ge 1 ]] ; do
  key="$1"
  case $key in
  -peer )
    PEER_NUMBER=$2
    shift
    ;;
  -posx )
    POSITION_X="$2"
    shift
    ;;
  -posy )
    POSITION_Y="$2"
    shift
    ;;
  -speed )
    Speed="$2"
    shift
    ;;
  * )
    echo "Unknown flag: $key"
    exit 1
    ;;
  esac
  shift
done

if [ "$MODE" == "createAsset" ]; then
    CreateAsset
elif [ "$MODE" == "caculateReliability" ]; then
    CaculateReliability
elif [ "$MODE" == "queryRobotspeed" ]; then
    QueryRobotSpeed
elif [ "$MODE" == "queryAveRobotspeed" ]; then
    QueryAveRobotSpeed
else
    exit 1
fi
