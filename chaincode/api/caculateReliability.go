package api

import (
	"encoding/json"
	"math"

	"github.com/hyperledger/fabric/core/chaincode/shim"
	pb "github.com/hyperledger/fabric/protos/peer"
)

func CaculateReliability(stub shim.ChaincodeStubInterface) pb.Response {

	alpha := []float64{2.3, 3, 2.8, 3.1, 2, 2.4}
	beta := []float64{600, 500, 600, 630, 500, 660}
	taus := []float64{10, 20}
	num_stage := 2
	acce_factors := []float64{1, 1.5}
	ks := []int{5, 4}

	Fj := make([][]float64, num_stage)
	for i := range Fj {
		Fj[i] = make([]float64, len(alpha))
	}

	Qj := make([][]float64, num_stage)
	for i := range Qj {
		Qj[i] = make([]float64, len(alpha))
	}

	Pj := make([][]float64, num_stage)
	for i := range Pj {
		Pj[i] = make([]float64, len(alpha))
	}
	number := 0
	sum_c := 0.0
	for i := 0; i < len(alpha); i++ {
		sum_c = 0
		for j := 0; j < num_stage; j++ {
			sum_c = sum_c + acce_factors[j]*taus[j]
			Fj[j][i] = 1 - math.Exp(-math.Pow(sum_c/beta[i], alpha[i]))
			Qj[j][i] = Fj[j][i]
			Pj[j][i] = 1 - Qj[j][i]
		}
	}

	Q := make([][]float64, num_stage)
	for i := range Q {
		Q[i] = make([]float64, len(alpha))
	}

	Pm := make([]float64, num_stage)
	Qm := make([]float64, num_stage)
	qm := make([]float64, num_stage)
	pm := make([]float64, num_stage)

	for m := 0; m < num_stage; m++ {
		for i := 0; i < len(alpha); i++ {
			if i == 0 {
				Q[m][i] = (1 - Qj[m][i])
			} else {
				Q[m][i] = Q[m][i-1] * (1 - Qj[m][i])
			}
		}
		Pm[m] = Q[m][len(alpha)-1]
		Qm[m] = 1 - Pm[m]
		if m == 0 {
			qm[m] = Qm[m]
		} else {
			qm[m] = Qm[m] - Qm[m-1]
		}
		pm[m] = 1 - qm[m]
	}

	Pn := make([]float64, 31)
	I := make([]float64, 31)

	for n := 5; n <= 30; n++ {
		Pr := make([]float64, num_stage)
		for j := 0; j < num_stage; j++ {
			P := 0.0
			for k := ks[j]; k <= n; k++ {
				P = P + float64(binomial(n, k))*math.Pow(1-qm[j], float64(k))*math.Pow(0.985*qm[j], float64(n-k))
			}
			if j == 0 {
				Pr[j] = P
			} else {
				Pr[j] = Pr[j-1] * P
			}
		}
		Pn[n] = Pr[num_stage-1]
		if n == 5 {
			I[n] = Pn[n]
		} else {
			I[n] = (Pn[n] - Pn[n-1]) / Pn[n-1]
		}
		if I[n] < 0 && I[n-1] >= 0 {
			number = n
		}
	}
	numberByte, _ := json.Marshal(number)
	return shim.Success(numberByte)
}

func binomial(n, k int) int {
	if k > n-k {
		k = n - k
	}
	b := 1
	for i := 1; i <= k; i++ {
		b *= n - (k - i)
		b /= i
	}
	return b
}
