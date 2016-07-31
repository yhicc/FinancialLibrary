
#include "Contract.h"
#include "BaseContract.h"
#include "IRSwapContract.h"
#include <string>


class Contract::ContractImpl{
public:
	
	ContractImpl(std::string &product);
	void SetContractInfo(const std::string &effective_date, const std::string &currency,
							int fixed_or_float, double notional_amount, double contract_term,
							double payment_period, double fixed_rate, double next_float_rate);	
	double CalcPV(const std::string &valuation_date, const std::vector<int> &floating_rate_term, const std::vector<double> &floating_rate_value, 
					const std::vector<int> &discount_curve_term, const std::vector<double> &discount_curve_value);
	
	std::unique_ptr<BaseContract> m_contract;
	void CreateContract(const std::string &product);
	
};


//constructor
Contract::Contract(const std::string &product){
	if(product == "IRS"){
		m_contract_impl.reset(new ContractImpl());
		m_contract_impl->CreateContract(product);
	}
//	else if{
//		
//	}
}

//Destructor
Contract::~Contract(){
}

void Contract::SetContractInfo(const std::string &effective_date, const std::string &currency,
						int fixed_or_float, double notional_amount, double contract_term,
						double payment_period, double fixed_rate, double next_float_rate){
	m_contract_impl->SetContractInfo((effective_date, currency, fixed_or_float, notional_amount, 
						contract_term, payment_period, fixed_rate, next_float_rate);
}

//calcPV Func
double Contract::CalcPV(const std::string &valuation_date, const std::vector<int> &floating_rate_term, const std::vector<double> &floating_rate_value, 
					const std::vector<int> &discount_curve_term, const std::vector<double> &discount_curve_value){
	m_contract_impl->CalcPV(valuation_date, floating_rate_term, floating_rate_value, 
							discount_curve_term, discount_curve_value);
}


////////////
//ContractImpl methods from here
////////////
void Contract::ContractImpl::SetContractInfo(const std::string &effective_date, const std::string &currency,
						int fixed_or_float, double notional_amount, double contract_term,
						double payment_period, double fixed_rate, double next_float_rate){
	m_contract->SetContractInfo((effective_date, currency, fixed_or_float, notional_amount, 
						contract_term, payment_period, fixed_rate, next_float_rate);
}

//calcPV Func
double Contract::ContractImpl::CalcPV(const std::string &valuation_date, const std::vector<int> &floating_rate_term, const std::vector<double> &floating_rate_value, 
					const std::vector<int> &discount_curve_term, const std::vector<double> &discount_curve_value){
	m_contract->CalcPV(valuation_date, floating_rate_term, floating_rate_value, 
							discount_curve_term, discount_curve_value);
}

void Contract::ContractImpl::CreateContract(const std::string &product){
	m_contract.reset(new IRSwapContract);
}


