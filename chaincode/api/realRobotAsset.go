package api

import (
	"chaincode/model"
	"chaincode/pkg/utils"
	"encoding/json"
	"fmt"
	"strconv"

	"github.com/hyperledger/fabric/core/chaincode/shim"
	pb "github.com/hyperledger/fabric/protos/peer"
)

// createAsset新建robot提交的信息
func CreateAsset(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	// 验证参数
	if len(args) != 4 {
		return shim.Error("参数个数不满足")
	}
	RobotId := args[0]   //RobotID
	PositionX := args[1] //位置x
	PositionY := args[2] //位置y
	Speed := args[3]     //速度
	if RobotId == "" || PositionX == "" || PositionY == "" || Speed == "" {
		return shim.Error("参数存在空值")
	}
	//参数格式转换
	// 参数数据格式转换
	var formattedSpeed float64
	if val, err := strconv.ParseFloat(Speed, 64); err != nil {
		return shim.Error(fmt.Sprintf("Speed参数格式转换出错: %s", err))
	} else {
		formattedSpeed = val
	}
	robotAsset := &model.RobotAsset{
		RobotId:   RobotId,
		PositionX: PositionX,
		PositionY: PositionY,
		Speed:     formattedSpeed,
	}
	//写入账本
	//now:=time.Now()
	//timestamp:=now.Format("20060102150405")
	if err := utils.WriteLedger(robotAsset, stub, model.RobotKey, []string{robotAsset.RobotId}); err != nil {
		return shim.Error(fmt.Sprintf("%s", err))
	}

	//将成功创建的信息返回
	robotAssetByte, err := json.Marshal(robotAsset)
	if err != nil {
		return shim.Error(fmt.Sprintf("序列化成功创建的信息出错: %s", err))
	}


	// 成功返回
	return shim.Success(robotAssetByte)
}

// 查询robot的速度信息
func QueryRobotSpeed(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	var robotspeedList []float64
	robotid:=args[0]
	results, err := utils.GetStateByPartialCompositeKeys2(stub, model.RobotKey, []string{robotid})
	if err != nil {
		return shim.Error(fmt.Sprintf("%s", err))
	}
	for _, v := range results {
		if v != nil {
			var robotasset model.RobotAsset
			err := json.Unmarshal(v, &robotasset)
			if err != nil {
				return shim.Error(fmt.Sprintf("QueryRealEstateList-反序列化出错: %s", err))
			}
			robotspeedList = append(robotspeedList, robotasset.Speed)
		}
	}
	robotspeedListByte, err := json.Marshal(robotspeedList)
	if err != nil {
		return shim.Error(fmt.Sprintf("QueryRealEstateList-序列化出错: %s", err))
	}
	return shim.Success(robotspeedListByte)
}

//平均速度
func QueryAveRobotsSpeed(stub shim.ChaincodeStubInterface) pb.Response {
	var totalSpeed float64
	var args []string
	k := 0
	results, err := utils.GetStateByPartialCompositeKeys(stub, model.RobotKey, args)
	if err != nil {
		return shim.Error(fmt.Sprintf("%s", err))
	}
	for _, v := range results {
		if v != nil {
			var robotasset model.RobotAsset
			err := json.Unmarshal(v, &robotasset)
			if err != nil {
				return shim.Error(fmt.Sprintf("QueryRealEstateList-反序列化出错: %s", err))
			}
			totalSpeed += robotasset.Speed
			k += 1
		}
	}
	aveSpeed := totalSpeed / float64(k)
	aveSpeedByte, err := json.Marshal(aveSpeed)
	if err != nil {
		return shim.Error(fmt.Sprintf("QueryAllRobotsSpeed-序列化出错: %s", err))
	}
	return shim.Success(aveSpeedByte)
}

/*
func QueryAsset(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	// 验证参数
	if len(args) != 1 {
		return shim.Error("参数个数不满足")
	}
	RobotId := args[0]   //RobotID
	if RobotId == "" {
		return shim.Error("参数存在空值")
	}
	//从账本中读取数据
	robotAssetByte, err := stub.GetState(RobotId)
	if err != nil {
		return shim.Error(fmt.Sprintf("从账本读取数据出错: %s", err))
	}
	if robotAssetByte == nil {
		return shim.Error(fmt.Sprintf("未找到相关数据%s", err))
	}
	// 成功返回
	return shim.Success(robotAssetByte)
}
*/
/*
type Asset struct {
	ID         string `json:"id"`
	PositionX  string `json:"positionX"`
	PositionY  string `json:"positionY"`
	Speed string `json:"speed"`
}

func CreateAsset(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	if len(args) != 4 {
		return shim.Error("Incorrect number of arguments. Expecting 4")
	}

	var asset = Asset{ID: args[0], PositionX: args[1], PositionY: args[2], Speed: args[3]}

	assetAsBytes, _ := json.Marshal(asset)
	err := stub.PutState(args[0], assetAsBytes)
	if err != nil {
		return shim.Error(fmt.Sprintf("Failed to create asset: %s", args[0]))
	}

	return shim.Success(nil)
}

func QueryAsset(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	if len(args) != 1 {
		return shim.Error("Incorrect number of arguments. Expecting 1")
	}

	assetAsBytes, _ := stub.GetState(args[0])
	if assetAsBytes == nil {
		return shim.Error(fmt.Sprintf("Could not locate asset: %s", args[0]))
	}

	return shim.Success(assetAsBytes)
}
*/
